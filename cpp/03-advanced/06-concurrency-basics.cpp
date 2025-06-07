// This program demonstrates basic C++ concurrency features.
#include <iostream>
#include <thread>       // For std::thread
#include <vector>
#include <mutex>        // For std::mutex, std::lock_guard, std::unique_lock
#include <future>       // For std::async, std::future, std::promise
#include <atomic>       // For std::atomic
#include <chrono>       // For std::chrono::milliseconds
#include <stdexcept>    // For std::runtime_error
#include <numeric>      // For std::accumulate

// --- Shared Data and Mutex ---
// Data races occur when multiple threads access shared data concurrently,
// and at least one access is a write, without synchronization.
// Mutexes are used to prevent data races by allowing only one thread
// to access a critical section (shared data) at a time.
std::mutex g_cout_mutex; // For synchronizing std::cout
std::mutex g_data_mutex;
int g_shared_data = 0;

// --- 1. std::thread ---
void thread_function_simple() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate work
    // Use lock_guard for cout to prevent garbled output
    std::lock_guard<std::mutex> lock(g_cout_mutex);
    std::cout << "Hello from simple thread: " << std::this_thread::get_id() << std::endl;
}

void thread_function_args(int id, const std::string& msg) {
    std::this_thread::sleep_for(std::chrono::milliseconds(id * 5));
    std::lock_guard<std::mutex> lock(g_cout_mutex);
    std::cout << "Thread " << id << " says: " << msg << " (ID: " << std::this_thread::get_id() << ")" << std::endl;
}

// --- 2. std::mutex and std::lock_guard ---
// std::lock_guard is an RAII wrapper for std::mutex. It locks the mutex in its constructor
// and unlocks it in its destructor, ensuring the mutex is always released.
void critical_section_worker(int id) {
    std::lock_guard<std::mutex> cout_lock(g_cout_mutex); // Lock for cout
    std::cout << "Thread " << id << " attempting to lock g_data_mutex." << std::endl;
    cout_lock.unlock(); // Unlock cout before potentially blocking on g_data_mutex

    { // Critical section for g_shared_data
        std::lock_guard<std::mutex> data_lock(g_data_mutex);
        g_shared_data++;
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Simulate work inside critical section

        // Re-lock cout_mutex to print safely
        std::lock_guard<std::mutex> cout_lock_again(g_cout_mutex);
        std::cout << "Thread " << id << " incremented g_shared_data to " << g_shared_data
                  << ". (g_data_mutex locked)" << std::endl;
    } // data_lock goes out of scope, g_data_mutex is unlocked.

    // Deadlocks can occur if threads try to acquire multiple locks in different orders.
    // E.g., Thread A locks M1 then M2, Thread B locks M2 then M1.
    // Always lock mutexes in the same order to prevent deadlocks.
}

// std::unique_lock is more flexible than std::lock_guard.
// It allows deferred locking, timed locking (try_lock_for, try_lock_until),
// recursive locking (with std::recursive_mutex), and manual lock/unlock.
void unique_lock_demo() {
    std::unique_lock<std::mutex> lock(g_data_mutex, std::defer_lock); // Does not lock initially

    std::{
        std::lock_guard<std::mutex> cout_lock(g_cout_mutex);
        std::cout << "[unique_lock_demo] Mutex not yet locked." << std::endl;
    }

    if (lock.try_lock()) { // Attempt to lock
        std::lock_guard<std::mutex> cout_lock(g_cout_mutex);
        std::cout << "[unique_lock_demo] Mutex acquired by try_lock(). Data: " << g_shared_data << std::endl;
        lock.unlock(); // Can be unlocked manually
        std::cout << "[unique_lock_demo] Mutex manually unlocked." << std::endl;
    } else {
        std::lock_guard<std::mutex> cout_lock(g_cout_mutex);
        std::cout << "[unique_lock_demo] try_lock() failed." << std::endl;
    }

    lock.lock(); // Lock it again (blocks if necessary)
    std::lock_guard<std::mutex> cout_lock(g_cout_mutex);
    std::cout << "[unique_lock_demo] Mutex locked again. Data: " << g_shared_data << std::endl;
    // lock will be automatically unlocked when it goes out of scope
}


// --- 3. std::async, std::future ---
// std::async launches a function (or callable) asynchronously, potentially in a new thread.
// It returns a std::future, which will eventually hold the result of the function call.
long long async_sum_task(int start, int end) {
    long long sum = 0;
    for (int i = start; i <= end; ++i) {
        sum += i;
    }
    std::lock_guard<std::mutex> lock(g_cout_mutex);
    std::cout << "[async_sum_task] Calculated sum from " << start << " to " << end << " is " << sum << std::endl;
    return sum;
}

// std::promise allows setting a value (or exception) that can be retrieved by a std::future.
// Useful for custom synchronization or when one thread produces a result another consumes.
void promise_task(std::promise<std::string>&& p, int delay_ms) {
    try {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        if (delay_ms > 500) {
            throw std::runtime_error("Promise task took too long!");
        }
        p.set_value("Data from promise after " + std::to_string(delay_ms) + "ms");
    } catch (...) {
        p.set_exception(std::current_exception()); // Propagate exception to the future
    }
}


// --- 4. std::atomic ---
// std::atomic types provide lock-free atomic operations on simple types,
// preventing data races for these specific operations without explicit mutexes.
// Useful for counters, flags, etc.
std::atomic<int> g_atomic_counter(0);

void atomic_increment_worker() {
    for (int i = 0; i < 10000; ++i) {
        g_atomic_counter++; // Atomic increment, no data race
    }
}


int main() {
    std::cout << "--- 1. std::thread Demo ---" << std::endl;
    std::thread t1(thread_function_simple);
    std::thread t2(thread_function_args, 101, "Hello from Thread 101");
    std::thread t3(thread_function_args, 102, "Greetings from Thread 102");
    // Threads must be joined or detached before their std::thread object is destroyed.
    // join() blocks until the thread finishes.
    // detach() lets the thread run independently (use with caution, ensure lifetime management).
    t1.join();
    t2.join();
    t3.join();
    std::cout << "All std::thread demos finished." << std::endl;

    std::cout << "\n--- 2. std::mutex & std::lock_guard Demo ---" << std::endl;
    g_shared_data = 0; // Reset shared data
    std::vector<std::thread> mutex_threads;
    for (int i = 0; i < 3; ++i) {
        mutex_threads.emplace_back(critical_section_worker, i + 1);
    }
    for (auto& t : mutex_threads) {
        t.join();
    }
    std::cout << "Final g_shared_data: " << g_shared_data << " (expected 3 if no races)" << std::endl;

    std::cout << "\n--- std::unique_lock Demo ---" << std::endl;
    std::thread t_unique_lock(unique_lock_demo);
    t_unique_lock.join();


    std::cout << "\n--- 3. std::async, std::future, std::promise Demo ---" << std::endl;
    // std::async can run a task in a new thread (std::launch::async)
    // or synchronously when the future's get() is called (std::launch::deferred).
    // Default policy is implementation-defined (usually std::launch::async | std::launch::deferred).
    std::future<long long> sum_future1 = std::async(std::launch::async, async_sum_task, 1, 100);
    std::future<long long> sum_future2 = std::async(std::launch::async, async_sum_task, 101, 200);

    // Do other work...
    std::cout << "Main thread doing other work while async tasks run..." << std::endl;

    // future::get() blocks until the result is available and returns it.
    // It can only be called once on a future.
    long long total_sum = sum_future1.get() + sum_future2.get();
    std::cout << "Total sum from async tasks: " << total_sum << std::endl;

    // std::promise demo
    std::promise<std::string> data_promise;
    std::future<std::string> data_future = data_promise.get_future();
    std::thread promise_thread(promise_task, std::move(data_promise), 300); // Launch promise task

    std::cout << "Main thread waiting for data from promise..." << std::endl;
    try {
        std::string promised_data = data_future.get(); // Blocks until promise sets value or exception
        std::cout << "Received from promise: " << promised_data << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception from promise_task: " << e.what() << std::endl;
    }
    promise_thread.join();


    std::cout << "\n--- 4. std::atomic Demo ---" << std::endl;
    g_atomic_counter = 0; // Reset
    std::vector<std::thread> atomic_threads;
    for (int i = 0; i < 5; ++i) { // 5 threads, each incrementing 10000 times
        atomic_threads.emplace_back(atomic_increment_worker);
    }
    for (auto& t : atomic_threads) {
        t.join();
    }
    std::cout << "Final g_atomic_counter: " << g_atomic_counter << " (expected 50000)" << std::endl;

    std::cout << "\nConcurrency basics demonstration complete." << std::endl;
    return 0;
}
