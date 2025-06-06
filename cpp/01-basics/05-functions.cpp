// This program demonstrates the definition and use of functions in C++.
#include <iostream>

// Function declaration
int add(int a, int b);

int main() {
    int sum = add(3, 4);
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}

// Function definition
int add(int a, int b) {
    return a + b;
}
