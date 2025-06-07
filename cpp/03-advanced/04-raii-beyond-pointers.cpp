// This program demonstrates RAII (Resource Acquisition Is Initialization)
// applied to resources other than raw memory, like file handles and mutexes.
#include <iostream>
#include <fstream>  // For std::ifstream, std::ofstream
#include <string>
#include <stdexcept> // For std::runtime_error
#include <mutex>     // For std::mutex, std::lock_guard (though we'll make our own)
#include <thread>    // For std::thread
#include <vector>    // For std::vector

// --- Example 1: RAII File Wrapper ---
class FileHandler {
private:
    std::fstream file_stream; // Using fstream for simplicity to handle both read/write if needed
    std::string filename;

public:
    // Constructor: Acquires the resource (opens the file)
    FileHandler(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        : filename(name) {
        file_stream.open(filename, mode);
        if (!file_stream.is_open()) {
            throw std::runtime_error("FileHandler: Failed to open file: " + filename);
        }
        std::cout << "FileHandler: File '" << filename << "' opened." << std::endl;
    }

    // Destructor: Releases the resource (closes the file)
    // This is automatically called when a FileHandler object goes out of scope,
    // ensuring the file is closed even if exceptions occur.
    ~FileHandler() {
        if (file_stream.is_open()) {
            file_stream.close();
            std::cout << "FileHandler: File '" << filename << "' closed." << std::endl;
        } else {
            std::cout << "FileHandler: File '" << filename << "' was already closed or not opened." << std::endl;
        }
    }

    // Example methods to use the resource
    void write(const std::string& text) {
        if (!file_stream.is_open() || !file_stream.good()) {
             throw std::runtime_error("FileHandler: File not open or in bad state for writing.");
        }
        // Go to end to append, or manage position as needed
        // file_stream.seekp(0, std::ios_base::end);
        file_stream << text << std::endl;
        std::cout << "FileHandler: Wrote to '" << filename << "': " << text << std::endl;
    }

    std::string readLine() {
        if (!file_stream.is_open() || !file_stream.good()) {
            throw std::runtime_error("FileHandler: File not open or in bad state for reading.");
        }
        // file_stream.seekg(0, std::ios_base::beg); // Go to beginning to read, or manage position
        std::string line;
        if (std::getline(file_stream, line)) {
            std::cout << "FileHandler: Read from '" << filename << "': " << line << std::endl;
            return line;
        }
        if (file_stream.eof()) {
             std::cout << "FileHandler: EOF reached for '" << filename << "'." << std::endl;
             file_stream.clear(); // Clear EOF flags to allow further operations if needed (e.g. seek)
             file_stream.seekg(0, std::ios_base::beg); // Reset to beginning for next read
        }
        return ""; // Or throw an error if read failed unexpectedly
    }

    // Prevent copying and assignment to maintain unique ownership of the file handle
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
};


// --- Example 2: RAII Mutex Lock Wrapper ---
// This is a simplified version of std::lock_guard
class MutexGuard {
private:
    std::mutex& mtx; // Reference to the mutex it manages
    bool locked;     // To ensure unlock is called only if lock was successful

public:
    // Constructor: Acquires the resource (locks the mutex)
    explicit MutexGuard(std::mutex& m) : mtx(m), locked(false) {
        mtx.lock();
        locked = true;
        std::cout << "MutexGuard: Mutex locked (Thread ID: " << std::this_thread::get_id() << ")" << std::endl;
    }

    // Destructor: Releases the resource (unlocks the mutex)
    // This is automatically called when a MutexGuard object goes out of scope.
    ~MutexGuard() {
        if (locked) {
            mtx.unlock();
            std::cout << "MutexGuard: Mutex unlocked (Thread ID: " << std::this_thread::get_id() << ")" << std::endl;
        }
    }

    // Prevent copying and assignment
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
};

// Global mutex and shared data for the threading example
std::mutex shared_data_mutex;
int shared_data = 0;

void worker_thread_function(int id) {
    try {
        std::cout << "Thread " << id << " trying to lock mutex." << std::endl;
        MutexGuard lock(shared_data_mutex); // RAII: Mutex is locked here
                                            // It will be automatically unlocked when 'lock' goes out of scope.

        // Critical section: Access shared data
        shared_data++;
        std::cout << "Thread " << id << " incremented shared_data to: " << shared_data << std::endl;

        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(10 * id));

        if (id == 2) { // Simulate an exception in one of the threads
            std::cout << "Thread " << id << " is throwing an exception inside critical section!" << std::endl;
            throw std::runtime_error("Simulated error in thread " + std::to_string(id));
        }
        std::cout << "Thread " << id << " finished work, releasing mutex implicitly." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Thread " << id << " caught an exception: " << e.what() << std::endl;
        // Even if an exception occurs, the MutexGuard destructor will be called
        // during stack unwinding, ensuring the mutex is unlocked.
    }
}


int main() {
    std::cout << "--- RAII FileHandler Demo ---" << std::endl;
    try {
        FileHandler my_file("raii_example.txt", std::ios::out | std::ios::trunc); // Create/truncate
        my_file.write("Hello RAII!");
        my_file.write("This ensures file is closed.");
        // my_file goes out of scope here, destructor closes the file.
    } catch (const std::exception& e) {
        std::cerr << "FileHandler demo caught exception: " << e.what() << std::endl;
    }

    try {
        FileHandler reader("raii_example.txt", std::ios::in); // Open for reading
        reader.readLine();
        reader.readLine();
        // Simulating an error after which the FileHandler destructor must still run
        // throw std::runtime_error("Simulated error after reading from file.");
    } catch (const std::exception& e) {
        std::cerr << "FileHandler demo (read) caught exception: " << e.what() << std::endl;
    }
    std::cout << "File 'raii_example.txt' should be closed now if it was opened." << std::endl;


    std::cout << "\n--- RAII MutexGuard Demo ---" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(worker_thread_function, i);
    }

    for (std::thread& t : threads) {
        t.join();
    }

    std::cout << "All threads joined. Final shared_data: " << shared_data << std::endl;
    std::cout << "RAII ensures mutexes were unlocked even if threads threw exceptions." << std::endl;

    std::cout << "\nRAII beyond pointers demonstration complete." << std::endl;
    return 0;
}
