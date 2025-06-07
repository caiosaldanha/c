// This program demonstrates std::vector usage in C++.
#include <iostream>
#include <vector>   // Required for std::vector
#include <algorithm> // For std::sort, std::find (though not explicitly requested, good for context)

int main() {
    // 1. Declaration and Initialization
    std::cout << "--- Declaration and Initialization ---" << std::endl;
    std::vector<int> vec1; // Empty vector of integers
    std::vector<std::string> vec2 = {"apple", "banana", "cherry"}; // Initialized with values
    std::vector<int> vec3(5, 100); // Vector of 5 integers, all initialized to 100
    std::vector<int> vec4 = {1, 2, 3, 4, 5};

    std::cout << "vec1 (empty): ";
    for (int val : vec1) std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "vec2: ";
    for (const std::string& s : vec2) std::cout << s << " ";
    std::cout << std::endl;

    std::cout << "vec3: ";
    for (int val : vec3) std::cout << val << " ";
    std::cout << std::endl;

    // 2. Adding Elements
    std::cout << "\n--- Adding Elements ---" << std::endl;
    vec1.push_back(10); // Add to the end
    vec1.push_back(20);
    vec1.push_back(30);
    std::cout << "vec1 after push_back: ";
    for (int val : vec1) std::cout << val << " ";
    std::cout << std::endl;

    // Insert element at a specific position
    // std::vector<int>::iterator it_insert = vec1.begin() + 1; // Iterator to the second element
    auto it_insert = vec1.begin() + 1; // Using auto for iterator type
    vec1.insert(it_insert, 15); // Insert 15 before the element at vec1.begin() + 1
    std::cout << "vec1 after insert(15 at index 1): ";
    for (int val : vec1) std::cout << val << " ";
    std::cout << std::endl;

    // 3. Removing Elements
    std::cout << "\n--- Removing Elements ---" << std::endl;
    if (!vec1.empty()) {
        vec1.pop_back(); // Remove the last element
        std::cout << "vec1 after pop_back: ";
        for (int val : vec1) std::cout << val << " ";
        std::cout << std::endl;
    }

    // Erase element at a specific position
    if (vec1.size() > 1) {
        // std::vector<int>::iterator it_erase = vec1.begin(); // Iterator to the first element
        auto it_erase = vec1.begin();
        vec1.erase(it_erase); // Erase the first element
        std::cout << "vec1 after erase (first element): ";
        for (int val : vec1) std::cout << val << " ";
        std::cout << std::endl;
    }

    // Erase a range of elements
    vec4.erase(vec4.begin() + 1, vec4.begin() + 3); // Erase elements from index 1 up to (but not including) index 3
    std::cout << "vec4 after erasing elements from index 1 to 2: ";
    for (int val : vec4) std::cout << val << " ";
    std::cout << std::endl;


    // 4. Accessing Elements
    std::cout << "\n--- Accessing Elements ---" << std::endl;
    if (!vec2.empty()) {
        std::cout << "First element of vec2 (index): " << vec2[0] << std::endl;
        std::cout << "First element of vec2 (at()): " << vec2.at(0) << std::endl;
        // .at() provides bounds checking and throws an std::out_of_range exception
        // Accessing the last element
        std::cout << "Last element of vec2: " << vec2.back() << std::endl;
        std::cout << "First element of vec2 (front()): " << vec2.front() << std::endl;
    }

    // 5. Getting Size and Capacity
    std::cout << "\n--- Size and Capacity ---" << std::endl;
    std::cout << "Size of vec2: " << vec2.size() << std::endl;
    std::cout << "Capacity of vec2: " << vec2.capacity() << std::endl; // Capacity is typically >= size
    vec2.push_back("date");
    std::cout << "Size of vec2 after push_back: " << vec2.size() << std::endl;
    std::cout << "Capacity of vec2 after push_back: " << vec2.capacity() << std::endl;
    vec2.shrink_to_fit(); // Requests the container to reduce its capacity to fit its size.
    std::cout << "Capacity of vec2 after shrink_to_fit: " << vec2.capacity() << std::endl;
    std::cout << "vec2 is " << (vec2.empty() ? "empty" : "not empty") << std::endl;


    // 6. Iterating Through a Vector
    std::cout << "\n--- Iterating Through a Vector ---" << std::endl;
    std::cout << "vec3 elements (index-based loop): ";
    for (size_t i = 0; i < vec3.size(); ++i) {
        std::cout << vec3[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "vec3 elements (range-based for loop): ";
    for (int val : vec3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "vec3 elements (iterator-based loop): ";
    // for (std::vector<int>::iterator it = vec3.begin(); it != vec3.end(); ++it) {
    for (auto it = vec3.begin(); it != vec3.end(); ++it) { // Using auto
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Clearing a vector
    std::cout << "\n--- Clearing a vector ---" << std::endl;
    std::cout << "vec3 size before clear: " << vec3.size() << std::endl;
    vec3.clear();
    std::cout << "vec3 size after clear: " << vec3.size() << std::endl;
    std::cout << "vec3 is " << (vec3.empty() ? "empty" : "not empty") << std::endl;


    return 0;
}
