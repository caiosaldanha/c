// This program demonstrates basic error handling in C++ using try-catch blocks and exceptions.
#include <iostream>
#include <string>
#include <stdexcept> // Required for standard exception classes like std::runtime_error, std::out_of_range
#include <vector>    // For an example that might throw std::out_of_range

// --- Custom Exception Class ---
// It's good practice to derive custom exceptions from std::exception
class MyCustomException : public std::exception {
private:
    std::string message;
public:
    MyCustomException(const std::string& msg) : message(msg) {}

    // Override what() to provide a description of the exception
    // const char* what() const noexcept override { // C++11 'override' and 'noexcept'
    // As per C++17, what() is no longer implicitly noexcept for derived classes unless specified
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// --- Function that might throw an exception ---
double divide(int numerator, int denominator) {
    if (denominator == 0) {
        // Throw a standard exception
        throw std::runtime_error("Division by zero error!");
    }
    if (numerator < 0 && denominator < 0) {
        // Throw a custom exception
        throw MyCustomException("Custom error: Both numerator and denominator are negative.");
    }
    return static_cast<double>(numerator) / denominator;
}

// --- RAII (Resource Acquisition Is Initialization) Example ---
// This class demonstrates RAII for managing a resource (e.g., a file or a lock)
class ResourceManager {
public:
    ResourceManager(const std::string& name) : resource_name(name) {
        std::cout << "Resource '" << resource_name << "' acquired." << std::endl;
        // In a real scenario, acquire the resource here (e.g., open file, lock mutex)
    }

    ~ResourceManager() { // Destructor
        std::cout << "Resource '" << resource_name << "' released (destructor called)." << std::endl;
        // Release the resource here. This is guaranteed to be called when the object goes out of scope,
        // whether by normal execution or due to an exception. This is C++'s equivalent of 'finally'.
    }

    void use() {
        std::cout << "Using resource '" << resource_name << "'." << std::endl;
    }
private:
    std::string resource_name;
};


int main() {
    std::cout << "--- Basic Try-Catch ---" << std::endl;
    try {
        double result = divide(10, 2);
        std::cout << "10 / 2 = " << result << std::endl;

        result = divide(5, 0); // This will throw std::runtime_error
        std::cout << "5 / 0 = " << result << std::endl; // This line won't be reached
    } catch (const std::runtime_error& e) {
        // Catching a specific standard exception by reference
        std::cerr << "Caught a runtime_error: " << e.what() << std::endl;
    }

    std::cout << "\n--- Catching Custom Exceptions ---" << std::endl;
    try {
        double result = divide(-10, -2); // This will throw MyCustomException
        std::cout << "-10 / -2 = " << result << std::endl; // This line won't be reached
    } catch (const MyCustomException& e) {
        std::cerr << "Caught MyCustomException: " << e.what() << std::endl;
    }

    std::cout << "\n--- Catching Multiple Exception Types ---" << std::endl;
    try {
        // Uncomment one of the following lines to test:
        // double result = divide(7, 0);       // Throws std::runtime_error
        // double result = divide(-7, -1);     // Throws MyCustomException
        std::vector<int> my_vector = {1, 2, 3};
        std::cout << "Accessing vector element: " << my_vector.at(5) << std::endl; // Throws std::out_of_range

    } catch (const std::out_of_range& e) {
        std::cerr << "Caught an out_of_range error: " << e.what() << std::endl;
    } catch (const MyCustomException& e) {
        std::cerr << "Caught MyCustomException: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) { // More general standard exceptions can be caught after specific ones
        std::cerr << "Caught a runtime_error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // Catching the base std::exception class will catch any standard exception
        // not caught by previous specific handlers. It's a good general fallback.
        std::cerr << "Caught a general std::exception: " << e.what() << std::endl;
    } catch (...) {
        // Catch-all handler: catches any exception not caught by previous handlers.
        // Use sparingly, as it gives no information about the type of exception.
        std::cerr << "Caught an unknown exception type." << std::endl;
    }

    std::cout << "\n--- RAII Demonstration (C++ 'finally' equivalent) ---" << std::endl;
    try {
        std::cout << "Entering RAII try block." << std::endl;
        ResourceManager res_manager("DemoResource"); // Resource acquired
        res_manager.use();
        // Simulate an error condition
        if (true) { // Change to false to see normal execution path
             std::cout << "Simulating an error and throwing inside RAII block." << std::endl;
             throw std::runtime_error("Simulated error for RAII demo.");
        }
        std::cout << "Exiting RAII try block normally (will not happen if exception thrown)." << std::endl;
        // res_manager destructor called here if no exception
    } catch (const std::exception& e) {
        std::cerr << "Caught exception during RAII demo: " << e.what() << std::endl;
        // res_manager destructor is called before this catch block is entered,
        // as the object goes out of scope when the try block is exited due to an exception.
    }
    std::cout << "After RAII try-catch block." << std::endl;


    std::cout << "\nError handling demonstration complete." << std::endl;
    return 0;
}
