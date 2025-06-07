// This program demonstrates various C++ string operations.
#include <iostream>
#include <string> // Required for std::string
#include <vector> // Used for an example with string split

int main() {
    // 1. Declaration and Initialization
    std::string str1 = "Hello";
    std::string str2("World");
    std::string str3; // Empty string

    std::cout << "--- Declaration and Initialization ---" << std::endl;
    std::cout << "str1: " << str1 << std::endl;
    std::cout << "str2: " << str2 << std::endl;
    std::cout << "str3 (empty): " << str3 << std::endl;

    // 2. Accessing Characters
    std::cout << "\n--- Accessing Characters ---" << std::endl;
    if (!str1.empty()) {
        std::cout << "First character of str1: " << str1[0] << std::endl;
        std::cout << "Last character of str1: " << str1.back() << std::endl;
        // Change a character
        str1[1] = 'a';
        std::cout << "str1 after changing 'e' to 'a': " << str1 << std::endl;
        str1[1] = 'e'; // Change it back
    }

    // 3. String Concatenation
    std::cout << "\n--- String Concatenation ---" << std::endl;
    std::string str_concat = str1 + ", " + str2 + "!";
    std::cout << "Concatenated string: " << str_concat << std::endl;

    str3.append(" Appended text.");
    std::cout << "str3 after append: " << str3 << std::endl;


    // 4. Getting String Length
    std::cout << "\n--- Getting String Length ---" << std::endl;
    std::cout << "Length of str1: " << str1.length() << std::endl;
    std::cout << "Size of str2: " << str2.size() << std::endl; // length() and size() are synonyms

    // 5. Substring Extraction
    std::cout << "\n--- Substring Extraction ---" << std::endl;
    if (str_concat.length() >= 7) {
        std::string sub = str_concat.substr(7, 5); // substr(position, length)
        std::cout << "Substring of str_concat (from index 7, length 5): " << sub << std::endl;
    }

    // 6. Finding Characters or Substrings
    std::cout << "\n--- Finding Characters or Substrings ---" << std::endl;
    std::string search_in = "The quick brown fox jumps over the lazy dog.";
    std::string to_find = "fox";
    size_t found_pos = search_in.find(to_find);

    if (found_pos != std::string::npos) { // npos means "not found"
        std::cout << "'" << to_find << "' found at position: " << found_pos << std::endl;
    } else {
        std::cout << "'" << to_find << "' not found." << std::endl;
    }

    size_t find_char_pos = search_in.find('q');
    if (find_char_pos != std::string::npos) {
        std::cout << "'q' found at position: " << find_char_pos << std::endl;
    } else {
        std::cout << "'q' not found." << std::endl;
    }

    // Example: Find first and last occurrence
    std::string multi_find_str = "apple pie, apple juice";
    size_t first_apple = multi_find_str.find("apple");
    size_t last_apple = multi_find_str.rfind("apple"); // rfind for reverse find

    if (first_apple != std::string::npos) {
         std::cout << "First 'apple' in '" << multi_find_str << "' is at: " << first_apple << std::endl;
    }
    if (last_apple != std::string::npos) {
         std::cout << "Last 'apple' in '" << multi_find_str << "' is at: " << last_apple << std::endl;
    }


    // Other useful operations
    std::cout << "\n--- Other Useful Operations ---" << std::endl;
    std::string s_empty = "";
    std::string s_not_empty = "test";
    std::cout << "s_empty is " << (s_empty.empty() ? "empty" : "not empty") << std::endl;
    std::cout << "s_not_empty is " << (s_not_empty.empty() ? "empty" : "not empty") << std::endl;

    std::string for_compare1 = "abc";
    std::string for_compare2 = "abd";
    if (for_compare1.compare(for_compare2) == 0) {
        std::cout << for_compare1 << " is equal to " << for_compare2 << std::endl;
    } else if (for_compare1.compare(for_compare2) < 0) {
        std::cout << for_compare1 << " is less than " << for_compare2 << std::endl;
    } else {
        std::cout << for_compare1 << " is greater than " << for_compare2 << std::endl;
    }

    // C-style string conversion
    const char* c_str = str1.c_str();
    std::cout << "C-style string of str1: " << c_str << std::endl;

    return 0;
}
