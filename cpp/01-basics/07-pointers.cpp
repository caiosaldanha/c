// This program demonstrates the concept of pointers in C++.
#include <iostream>

int main() {
    int var = 20;     // Actual variable declaration
    int *ptr;         // Pointer variable declaration

    ptr = &var;       // Store the address of var in ptr

    std::cout << "Value of var: " << var << std::endl;
    std::cout << "Address of var: " << ptr << std::endl;
    std::cout << "Value pointed to by ptr: " << *ptr << std::endl;  // Dereference the pointer

    return 0;
}
