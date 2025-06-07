// This program demonstrates std::set usage in C++.
// std::set stores unique elements, sorted in ascending order by default.
#include <iostream>
#include <set>      // Required for std::set
#include <string>
#include <algorithm> // For std::set_union, std::set_intersection, std::inserter
#include <vector>    // For storing results of set operations

int main() {
    // 1. Declaration and Initialization
    std::cout << "--- Declaration and Initialization ---" << std::endl;
    std::set<int> set1; // Empty set of integers
    std::set<std::string> set2 = {"apple", "banana", "cherry", "apple"}; // "apple" will only appear once
    std::set<int, std::greater<int>> set3 = {5, 1, 8, 3, 5}; // Set with custom comparator (sorted descending)

    std::cout << "set1 (empty): ";
    for (int val : set1) std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "set2 (strings, default ascending order): ";
    for (const std::string& s : set2) std::cout << s << " "; // Elements are sorted
    std::cout << std::endl;

    std::cout << "set3 (integers, descending order): ";
    for (int val : set3) std::cout << val << " ";
    std::cout << std::endl;

    // 2. Inserting Elements
    std::cout << "\n--- Inserting Elements ---" << std::endl;
    set1.insert(10);
    set1.insert(5);
    set1.insert(20);
    set1.insert(5); // Inserting a duplicate element has no effect

    std::cout << "set1 after insertions: ";
    for (int val : set1) std::cout << val << " "; // Output: 5 10 20
    std::cout << std::endl;

    // The insert method returns a std::pair<iterator, bool>
    auto result = set1.insert(15);
    if (result.second) { // bool is true if insertion took place
        std::cout << "15 was inserted successfully. Current value at iterator: " << *(result.first) << std::endl;
    } else { // bool is false if element already existed
        std::cout << "15 already existed. Current value at iterator: " << *(result.first) << std::endl;
    }

    // 3. Checking if an Element Exists
    std::cout << "\n--- Checking if an Element Exists ---" << std::endl;
    int search_val = 10;
    // Using count() - returns 1 if element exists, 0 otherwise (since sets have unique keys)
    if (set1.count(search_val)) {
        std::cout << search_val << " exists in set1." << std::endl;
    } else {
        std::cout << search_val << " does not exist in set1." << std::endl;
    }

    search_val = 7;
    // Using find() - returns an iterator to the element, or .end() if not found
    // std::set<int>::iterator it_find = set1.find(search_val);
    auto it_find = set1.find(search_val); // Using auto
    if (it_find != set1.end()) {
        std::cout << search_val << " found in set1. Value: " << *it_find << std::endl;
    } else {
        std::cout << search_val << " not found in set1." << std::endl;
    }

    // 4. Iterating Through a Set
    std::cout << "\n--- Iterating Through a Set ---" << std::endl;
    std::cout << "set2 elements (range-based for loop): ";
    for (const std::string& s : set2) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    std::cout << "set1 elements (iterator-based loop): ";
    // for (std::set<int>::iterator it = set1.begin(); it != set1.end(); ++it) {
    for (auto it = set1.begin(); it != set1.end(); ++it) { // Using auto
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 5. Removing Elements
    std::cout << "\n--- Removing Elements ---" << std::endl;
    int val_to_remove = 10;
    size_t num_erased = set1.erase(val_to_remove); // Erase by value
    if (num_erased > 0) {
        std::cout << val_to_remove << " was removed. " << num_erased << " element(s) erased." << std::endl;
    } else {
        std::cout << val_to_remove << " was not found for removal." << std::endl;
    }
    std::cout << "set1 after removing 10: ";
    for (int val : set1) std::cout << val << " ";
    std::cout << std::endl;

    // Erase by iterator (more efficient if you already have an iterator)
    auto it_remove_5 = set1.find(5);
    if (it_remove_5 != set1.end()) {
        set1.erase(it_remove_5);
        std::cout << "5 removed using iterator." << std::endl;
    }
    std::cout << "set1 after removing 5: ";
    for (int val : set1) std::cout << val << " ";
    std::cout << std::endl;

    // 6. Set Operations (Union, Intersection)
    std::cout << "\n--- Set Operations ---" << std::endl;
    std::set<int> s_a = {1, 2, 3, 4, 5};
    std::set<int> s_b = {4, 5, 6, 7, 8};
    std::vector<int> s_union_result; // Use vector for storing results as set_union needs output iterator
    std::vector<int> s_intersection_result;

    // Union: elements that are in s_a or s_b or both
    std::set_union(s_a.begin(), s_a.end(),
                   s_b.begin(), s_b.end(),
                   std::back_inserter(s_union_result)); // Inserts at the back of the vector

    std::cout << "Union of s_a and s_b: ";
    for (int val : s_union_result) std::cout << val << " "; // Output: 1 2 3 4 5 6 7 8
    std::cout << std::endl;

    // Intersection: elements that are in both s_a and s_b
    std::set_intersection(s_a.begin(), s_a.end(),
                          s_b.begin(), s_b.end(),
                          std::back_inserter(s_intersection_result));

    std::cout << "Intersection of s_a and s_b: ";
    for (int val : s_intersection_result) std::cout << val << " "; // Output: 4 5
    std::cout << std::endl;

    // Difference (elements in s_a but not in s_b)
    std::vector<int> s_difference_result;
    std::set_difference(s_a.begin(), s_a.end(),
                        s_b.begin(), s_b.end(),
                        std::back_inserter(s_difference_result));
    std::cout << "Difference (s_a - s_b): ";
    for (int val : s_difference_result) std::cout << val << " "; // Output: 1 2 3
    std::cout << std::endl;


    // Size, empty, clear
    std::cout << "\n--- Size, Empty, Clear ---" << std::endl;
    std::cout << "set2 size: " << set2.size() << std::endl;
    std::cout << "set2 is " << (set2.empty() ? "empty" : "not empty") << std::endl;
    set2.clear();
    std::cout << "set2 after clear is " << (set2.empty() ? "empty" : "not empty") << std::endl;

    return 0;
}
