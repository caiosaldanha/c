// This program demonstrates std::map usage in C++.
#include <iostream>
#include <map>      // Required for std::map
#include <string>

int main() {
    // 1. Declaration and Initialization
    std::cout << "--- Declaration and Initialization ---" << std::endl;
    std::map<std::string, int> student_scores; // Map from student name (string) to score (int)
    std::map<int, std::string> error_codes = { // Initializer list (C++11)
        {404, "Not Found"},
        {500, "Internal Server Error"}
    };

    std::cout << "student_scores map is initially " << (student_scores.empty() ? "empty" : "not empty") << std::endl;
    std::cout << "error_codes map size: " << error_codes.size() << std::endl;

    // 2. Inserting Key-Value Pairs
    std::cout << "\n--- Inserting Key-Value Pairs ---" << std::endl;
    // Using operator[]
    student_scores["Alice"] = 95;
    student_scores["Bob"] = 88;

    // Using insert() method (returns a std::pair<iterator, bool>)
    auto result_pair = student_scores.insert(std::make_pair("Charlie", 92));
    if (result_pair.second) {
        std::cout << "Charlie inserted successfully with score " << result_pair.first->second << std::endl;
    } else {
        std::cout << "Charlie already exists with score " << result_pair.first->second << std::endl;
    }

    // Using insert with C++17 structured binding
    auto [it, success] = student_scores.insert_or_assign("Alice", 96); // Updates if exists, inserts if not
    if (success) {
         std::cout << "Alice inserted with score " << it->second << std::endl;
    } else {
         std::cout << "Alice's score updated to " << it->second << std::endl;
    }


    std::cout << "Student Scores after insertions:" << std::endl;
    for (const auto& pair : student_scores) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 3. Accessing Values by Key
    std::cout << "\n--- Accessing Values by Key ---" << std::endl;
    std::cout << "Bob's score (using operator[]): " << student_scores["Bob"] << std::endl;

    // Using at() - provides bounds checking (throws std::out_of_range if key not found)
    try {
        std::cout << "Alice's score (using at()): " << student_scores.at("Alice") << std::endl;
        // std::cout << "David's score (using at()): " << student_scores.at("David") << std::endl; // This would throw
    } catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << std::endl;
    }
    // Note: Using operator[] on a non-existent key will insert it with a default-constructed value.
    // For example, if "David" wasn't in student_scores, student_scores["David"] would add "David" with score 0.
    // std::cout << "David's score (operator[]): " << student_scores["David"] << std::endl; // David is now in map with score 0
    // std::cout << "Map size after accessing non-existent key with []: " << student_scores.size() << std::endl;


    // 4. Checking if a Key Exists
    std::cout << "\n--- Checking if a Key Exists ---" << std::endl;
    std::string search_key = "Charlie";
    // Using count()
    if (student_scores.count(search_key)) { // count returns 1 if key exists, 0 otherwise
        std::cout << search_key << " exists in the map. Score: " << student_scores[search_key] << std::endl;
    } else {
        std::cout << search_key << " does not exist in the map." << std::endl;
    }

    search_key = "Eve";
    // Using find()
    // std::map<std::string, int>::iterator it_find = student_scores.find(search_key);
    auto it_find = student_scores.find(search_key); // Using auto
    if (it_find != student_scores.end()) { // find() returns an iterator to element, or .end() if not found
        std::cout << search_key << " found! Score: " << it_find->second << std::endl;
    } else {
        std::cout << search_key << " not found." << std::endl;
    }

    // 5. Iterating Through a Map
    std::cout << "\n--- Iterating Through a Map ---" << std::endl;
    std::cout << "All student scores (range-based for loop):" << std::endl;
    for (const auto& pair : student_scores) { // Elements are std::pair<const Key, Value>
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }

    std::cout << "Error codes (iterator-based loop):" << std::endl;
    // for(std::map<int, std::string>::iterator it_loop = error_codes.begin(); it_loop != error_codes.end(); ++it_loop) {
    for(auto it_loop = error_codes.begin(); it_loop != error_codes.end(); ++it_loop) { // Using auto
        std::cout << "Code: " << it_loop->first << ", Message: " << it_loop->second << std::endl;
    }

    // 6. Removing Elements
    std::cout << "\n--- Removing Elements ---" << std::endl;
    std::string key_to_remove = "Bob";
    size_t num_erased = student_scores.erase(key_to_remove); // Erase by key
    if (num_erased > 0) {
        std::cout << key_to_remove << " was removed. " << num_erased << " element(s) erased." << std::endl;
    } else {
        std::cout << key_to_remove << " was not found for removal." << std::endl;
    }

    // Erase by iterator (more efficient if you already have an iterator)
    auto it_remove_charlie = student_scores.find("Charlie");
    if (it_remove_charlie != student_scores.end()) {
        student_scores.erase(it_remove_charlie);
        std::cout << "Charlie removed using iterator." << std::endl;
    }

    std::cout << "Student scores after removals:" << std::endl;
    for (const auto& pair : student_scores) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Size and clear
    std::cout << "\n--- Size and Clear ---" << std::endl;
    std::cout << "Current size of student_scores: " << student_scores.size() << std::endl;
    student_scores.clear();
    std::cout << "Size of student_scores after clear: " << student_scores.size() << std::endl;
    std::cout << "student_scores map is now " << (student_scores.empty() ? "empty" : "not empty") << std::endl;

    return 0;
}
