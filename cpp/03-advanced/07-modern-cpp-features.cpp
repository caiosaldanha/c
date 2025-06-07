// This program showcases a selection of modern C++ features.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::for_each, std::sort
#include <optional>  // For std::optional (C++17)
#include <functional> // For std::function

// --- 1. Lambda Expressions (Advanced) ---
class LambdaDemo {
public:
    int value = 10;

    void demo_lambdas() {
        std::cout << "--- Advanced Lambda Expressions ---" << std::endl;
        std::vector<int> numbers = {1, 2, 3, 4, 5};

        // a) Generic Lambdas (C++14 onwards, using 'auto' for parameters)
        auto print_doubled = [](const auto& n) { // 'auto' makes it generic
            std::cout << n * 2 << " ";
        };
        std::cout << "Generic lambda (print_doubled): ";
        std::for_each(numbers.begin(), numbers.end(), print_doubled);
        std::cout << std::endl;
        std::vector<double> doubles = {1.1, 2.2, 3.3};
        std::cout << "Generic lambda with doubles: ";
        std::for_each(doubles.begin(), doubles.end(), print_doubled);
        std::cout << std::endl;


        // b) Capturing 'this'
        // To access member variables of the enclosing class within a lambda.
        // [this] captures 'this' by pointer (access members like this->value)
        // [*this] (C++17) captures a copy of the current object (*this)
        auto member_access_lambda = [this](int x) {
            std::cout << "Lambda accessing member 'value': " << this->value << ", plus arg: " << x + this->value << std::endl;
        };
        member_access_lambda(5);

        // c) Mutable Lambdas
        // By default, lambdas are const-by-value for captured variables (unless captured by reference).
        // 'mutable' allows modification of by-value captured variables within the lambda.
        // The changes are local to the lambda's state.
        int counter = 0;
        auto mutable_lambda = [counter]() mutable {
            counter++;
            std::cout << "Mutable lambda: counter = " << counter << std::endl;
            return counter;
        };
        mutable_lambda(); // counter is 1
        mutable_lambda(); // counter is 2 (state is preserved across calls to this specific lambda object)
        std::cout << "Original counter after mutable lambda calls: " << counter << " (unchanged)" << std::endl;

        // d) Lambdas as Function Objects / Storing in std::function
        std::function<int(int, int)> add_func = [](int a, int b) -> int { // Explicit return type
            return a + b;
        };
        std::cout << "Lambda stored in std::function: add_func(3, 4) = " << add_func(3, 4) << std::endl;

        // Lambda as a comparator for sort
        std::vector<std::string> names = {"Charlie", "Alpha", "Bravo"};
        std::sort(names.begin(), names.end(), [](const std::string& a, const std::string& b){
            return a.length() < b.length(); // Sort by length
        });
        std::cout << "Names sorted by length: ";
        for(const auto& n : names) std::cout << n << " ";
        std::cout << std::endl;
    }
};


// --- 2. `auto` and `decltype` ---
void auto_decltype_demo() {
    std::cout << "\n--- `auto` and `decltype` ---" << std::endl;

    // a) `auto` for type inference
    auto i = 42;          // i is int
    auto d = 3.14;        // d is double
    auto s = std::string("hello"); // s is std::string
    auto vec = std::vector<int>{1,2,3}; // vec is std::vector<int>

    std::cout << "auto deduced types: i=" << i << " (int), d=" << d << " (double), s=" << s << " (string)" << std::endl;

    // `auto` with iterators
    std::cout << "Vector elements using auto iterator: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // b) `decltype` for deducing the type of an expression
    int x = 10;
    decltype(x) y = 20; // y is int (type of x)
    decltype(x * 0.5) z = 5.5; // z is double (type of expression x * 0.5)
    std::cout << "decltype deduced types: y=" << y << " (int), z=" << z << " (double)" << std::endl;

    // c) `decltype(auto)` (C++14) for perfect return type deduction in functions (especially templates)
    // Useful when the exact type (including references and const-qualifiers) needs to be preserved.
    auto get_first_element = [](const std::vector<int>& v) -> decltype(auto) { // Returns const int&
        if (v.empty()) throw std::out_of_range("Empty vector");
        return v.front();
    };
    const int& first = get_first_element(vec);
    std::cout << "decltype(auto) for return type: first element = " << first << std::endl;
    // vec[0] = 100; // Modifying would show if 'first' is a reference
    // std::cout << "First after modification: " << first << std::endl;
}

// --- 3. `constexpr` Functions ---
// `constexpr` functions can be evaluated at compile time if arguments are compile-time constants.
// This can improve performance and allow results to be used in contexts requiring compile-time constants.
constexpr long long factorial(int n) {
    if (n < 0) throw std::invalid_argument("Factorial of negative not allowed");
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

void constexpr_demo() {
    std::cout << "\n--- `constexpr` Functions ---" << std::endl;
    constexpr int num = 5;
    constexpr long long fact5 = factorial(num); // Calculated at compile time
    std::cout << "Factorial of " << num << " (compile-time): " << fact5 << std::endl;

    // Can also be called at runtime
    int runtime_num = 6;
    long long fact_runtime = factorial(runtime_num);
    std::cout << "Factorial of " << runtime_num << " (run-time): " << fact_runtime << std::endl;

    // Can be used for array sizes (if result is compile-time constant)
    // char arr[factorial(4)]; // arr size is 24, known at compile time
    // std::cout << "Size of array char arr[factorial(4)]: " << sizeof(arr) << std::endl;
}


// --- 4. `std::optional` (C++17) ---
// Represents an optional value that may or may not exist.
// Avoids using null pointers or special "magic" values (like -1 or empty string)
// to indicate absence of a value.
std::optional<std::string> find_user(int user_id) {
    if (user_id == 1) {
        return "Alice"; // Value exists
    } else if (user_id == 2) {
        return "Bob";   // Value exists
    }
    return std::nullopt; // Value does not exist (std::nullopt is like nullptr for optional)
}

void optional_demo() {
    std::cout << "\n--- `std::optional` (C++17) ---" << std::endl;

    std::optional<std::string> user1 = find_user(1);
    std::optional<std::string> user3 = find_user(3);

    // Check if value exists
    if (user1.has_value()) { // or simply: if (user1)
        std::cout << "User 1 found: " << user1.value() << std::endl; // .value() accesses the value
                                                                  // throws std::bad_optional_access if no value
    } else {
        std::cout << "User 1 not found." << std::endl;
    }

    if (user3) { // More idiomatic check
        std::cout << "User 3 found: " << *user3 << std::endl; // operator* also accesses value (unsafe if no value)
    } else {
        std::cout << "User 3 not found." << std::endl;
    }

    // Access with value_or (provides a default if no value)
    std::cout << "User 1 (value_or): " << user1.value_or("Default User") << std::endl;
    std::cout << "User 3 (value_or): " << user3.value_or("Default User") << std::endl;

    // Modifying the contained value
    if(user1){
        user1->append(" Wonderland"); // Access methods of std::string via operator->
        std::cout << "Modified User 1: " << *user1 << std::endl;
    }

    // Resetting an optional
    user1.reset(); // Makes it contain no value
    if(!user1){
        std::cout << "User 1 reset, now contains no value." << std::endl;
    }
}


int main() {
    LambdaDemo ld;
    ld.demo_lambdas();

    auto_decltype_demo();
    constexpr_demo();
    optional_demo();

    std::cout << "\nModern C++ features demonstration complete." << std::endl;
    return 0;
}
