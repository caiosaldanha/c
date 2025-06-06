// This program demonstrates variable declaration, initialization, and various data types in C++.
#include <iostream>
#include <string>

int main() {
    int myInt = 5;             // Integer type
    double myDouble = 5.99;    // Double (floating point) type
    char myChar = 'D';         // Character type
    bool myBool = true;        // Boolean type
    std::string myString = "Hello";  // String type (from the standard library)

    std::cout << myInt << "\n";
    std::cout << myDouble << "\n";
    std::cout << myChar << "\n";
    std::cout << myBool << "\n";
    std::cout << myString << "\n";

    return 0;
}
