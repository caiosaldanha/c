// This program demonstrates the declaration, initialization, and usage of arrays in C++.
#include <iostream>

int main() {
    int myArray[5] = {1, 2, 3, 4, 5};  // Declare an array of integers

    for (int i = 0; i < 5; i++) {
        std::cout << "Element " << i << ": " << myArray[i] << std::endl;
    }
    return 0;
}
