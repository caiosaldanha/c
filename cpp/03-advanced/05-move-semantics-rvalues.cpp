// This program provides a deeper dive into move semantics and rvalue references in C++.
#include <iostream>
#include <vector>
#include <string>
#include <utility> // For std::move, std::forward
#include <algorithm> // For std::swap in older C++ versions for swap idiom

// --- Custom String-like class to demonstrate move semantics ---
class MyString {
private:
    char* data;
    size_t len;

    void log(const std::string& message) const {
        std::cout << "[MyString] " << (data ? data : "(null)") << " (" << len << "): " << message << std::endl;
    }

public:
    // Default constructor
    MyString() : data(nullptr), len(0) {
        log("Default constructor");
    }

    // Constructor from C-string
    MyString(const char* s) {
        log("Constructor from C-string");
        if (s) {
            len = strlen(s);
            data = new char[len + 1];
            strcpy(data, s);
        } else {
            data = nullptr;
            len = 0;
        }
    }

    // 1. Copy Constructor (Deep Copy)
    MyString(const MyString& other) {
        log("Copy constructor from " + (other.data ? std::string(other.data) : "(null)"));
        if (other.data) {
            len = other.len;
            data = new char[len + 1];
            strcpy(data, other.data);
        } else {
            data = nullptr;
            len = 0;
        }
    }

    // 2. Copy Assignment Operator (Deep Copy, with copy-and-swap or basic check)
    MyString& operator=(const MyString& other) {
        log("Copy assignment operator from " + (other.data ? std::string(other.data) : "(null)"));
        if (this == &other) { // Self-assignment check
            return *this;
        }
        delete[] data; // Free existing resource

        if (other.data) {
            len = other.len;
            data = new char[len + 1];
            strcpy(data, other.data);
        } else {
            data = nullptr;
            len = 0;
        }
        return *this;
    }

    // --- Move Semantics ---
    // Rvalue references (&&) allow us to differentiate between lvalues (objects with names)
    // and rvalues (temporary objects, literals). This enables move semantics.

    // 3. Move Constructor
    // "Steals" the resources from the source object (other), leaving it in a valid but empty/unspecified state.
    // 'noexcept' is important for move operations used in STL containers for strong exception guarantees.
    MyString(MyString&& other) noexcept {
        log("Move constructor from " + (other.data ? std::string(other.data) : "(null)"));
        data = other.data; // Steal the pointer
        len = other.len;   // Steal the length

        // Leave the source object in a valid (destructible) but unspecified state
        other.data = nullptr;
        other.len = 0;
    }

    // 4. Move Assignment Operator
    MyString& operator=(MyString&& other) noexcept {
        log("Move assignment operator from " + (other.data ? std::string(other.data) : "(null)"));
        if (this == &other) { // Self-assignment check (though rare with rvalues)
            return *this;
        }
        delete[] data; // Free existing resource

        data = other.data; // Steal the pointer
        len = other.len;   // Steal the length

        // Leave the source object in a valid state
        other.data = nullptr;
        other.len = 0;
        return *this;
    }

    // Destructor
    ~MyString() {
        log("Destructor");
        delete[] data; // Safe to call delete[] on nullptr
    }

    void print() const {
        if (data) {
            std::cout << data << std::endl;
        } else {
            std::cout << "(empty)" << std::endl;
        }
    }

    size_t length() const { return len; }

    // Friend function for concatenation example
    friend MyString operator+(const MyString& lhs, const MyString& rhs);
};

MyString operator+(const MyString& lhs, const MyString& rhs) {
    std::cout << "[Global operator+] Concatenating..." << std::endl;
    size_t new_len = lhs.len + rhs.len;
    char* new_data = new char[new_len + 1];

    strcpy(new_data, lhs.data ? lhs.data : "");
    if (rhs.data) strcat(new_data, rhs.data);

    MyString result(new_data); // Creates a MyString from new_data (constructor handles copy)
    delete[] new_data; // MyString constructor made its own copy

    // If MyString(const char*) was changed to steal, this would be simpler.
    // Or, if MyString had a constructor MyString(char* buffer, size_t len, bool adopt_memory)
    // For now, this is fine to show how temporaries are created.
    // The key is that 'result' might be moved out if NRVO doesn't apply or if it's std::move'd
    return result; // Returns an rvalue
}

// --- Lvalues and Rvalues Recap ---
// Lvalue: An expression that refers to a memory location and can appear on the left-hand side of an assignment.
//         Basically, an object that has a name. E.g., `int x = 10;` (`x` is an lvalue).
// Rvalue: An expression that is not an lvalue. Often temporary values or literals.
//         E.g., `10`, `x + 5`, `MyString("temp")`. Rvalues typically appear on the right-hand side.

// --- Factory function for demonstrating Perfect Forwarding ---
// 5. Perfect Forwarding
// Allows a function template to pass its arguments to another function
// while preserving their value categories (lvalue/rvalue).
// Uses `std::forward` with template type deduction (forwarding references).

template<typename T, typename... Args> // Variadic template for arbitrary arguments
T create_object(Args&&... args) {      // Args&& is a "forwarding reference" or "universal reference"
    std::cout << "create_object factory: forwarding arguments to constructor of T" << std::endl;
    // std::forward<Args>(args)... correctly forwards arguments as lvalues or rvalues
    return T(std::forward<Args>(args)...);
}


int main() {
    std::cout << "--- Lvalues and Rvalues Examples ---" << std::endl;
    MyString s1("Hello"); // s1 is an lvalue
    MyString s2("World"); // s2 is an lvalue
    // MyString("temporary") // This is an rvalue (a temporary object)
    // s1 + s2               // The result of operator+ is an rvalue

    std::cout << "\n--- Copy Semantics ---" << std::endl;
    MyString s3 = s1; // Copy constructor (s1 is lvalue)
    MyString s4;
    s4 = s2;          // Copy assignment operator (s2 is lvalue)
    s3.print();
    s4.print();

    std::cout << "\n--- Move Semantics with std::move ---" << std::endl;
    // `std::move` casts an lvalue to an rvalue reference, enabling move operations.
    // After moving from an object, it should be considered to be in a valid but unspecified state.
    // Don't rely on its old value.

    MyString s5 = std::move(s1); // Move constructor (std::move(s1) is an rvalue reference)
    std::cout << "s5 (after move from s1): "; s5.print();
    std::cout << "s1 (after being moved from): "; s1.print(); // s1 is now "empty" or similar

    MyString s6;
    s6 = std::move(s2);          // Move assignment operator (std::move(s2) is an rvalue reference)
    std::cout << "s6 (after move from s2): "; s6.print();
    std::cout << "s2 (after being moved from): "; s2.print();

    std::cout << "\n--- Move Semantics with rvalue results from functions ---" << std::endl;
    MyString s7 = MyString("Temporary literal"); // Move constructor (MyString("...") is rvalue)
                                                 // This might be optimized by compiler (elision),
                                                 // but conceptually it's a move.
    s7.print();

    MyString s8 = s5 + s6; // operator+ returns an rvalue (a temporary MyString object)
                           // This temporary is then used to initialize s8, ideally via move constructor.
    std::cout << "s8 (result of s5 + s6): "; s8.print();


    std::cout << "\n--- Move Semantics in a Vector ---" << std::endl;
    std::vector<MyString> vec_str;
    // When vector reallocates, it uses move constructors (if available and noexcept)
    // to move elements to new memory, which is much more efficient than copying.
    std::cout << "Pushing lvalue (s3 - copy expected if not moved explicitly):" << std::endl;
    vec_str.push_back(s3); // Copy constructor (s3 is an lvalue)

    std::cout << "Pushing rvalue (temporary - move expected):" << std::endl;
    vec_str.push_back(MyString("Temporary for vector")); // Move constructor

    std::cout << "Pushing lvalue with std::move (move expected):" << std::endl;
    MyString s_for_vec("To be moved into vector");
    vec_str.push_back(std::move(s_for_vec)); // Move constructor
    std::cout << "s_for_vec after move: "; s_for_vec.print();

    std::cout << "Vector contents:" << std::endl;
    for (const auto& s : vec_str) {
        s.print();
    }

    std::cout << "\n--- Perfect Forwarding Demo ---" << std::endl;
    MyString pf1 = create_object<MyString>("Perfectly Forwarded C-string");

    MyString pf_source("Lvalue Source for PF");
    // MyString pf2 = create_object<MyString>(pf_source); // Calls Copy Constructor of MyString as pf_source is an Lvalue
    MyString pf2 = create_object<MyString>(std::move(pf_source)); // Calls Move Constructor as std::move(pf_source) is an Rvalue

    pf1.print();
    pf2.print();
    std::cout << "pf_source after being (potentially) moved by create_object: "; pf_source.print();


    std::cout << "\nMove semantics demonstration complete." << std::endl;
    return 0;
}
