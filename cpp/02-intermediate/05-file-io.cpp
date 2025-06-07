// This program demonstrates basic file I/O operations in C++.
#include <iostream>
#include <fstream>  // Required for std::ofstream (output file stream) and std::ifstream (input file stream)
#include <string>
#include <vector>

const std::string FILENAME = "example.txt"; // Define a constant for the filename

int main() {
    // --- Writing to a File (std::ofstream) ---
    std::cout << "--- Writing to File ---" << std::endl;
    // 1. Open the file for writing
    // std::ios::out is default for ofstream, std::ios::trunc overwrites if file exists
    std::ofstream outfile(FILENAME); // Opens FILENAME for writing, creates if not exists, truncates if exists

    // 2. Check if the file was opened successfully
    if (!outfile.is_open()) { // or !outfile
        std::cerr << "Error: Could not open file " << FILENAME << " for writing." << std::endl;
        return 1; // Indicate an error
    }

    // 3. Write data to the file
    outfile << "Hello, File!" << std::endl;
    outfile << "This is the second line." << std::endl;
    outfile << 42 << " is an integer." << std::endl;
    outfile << 3.14159 << " is a double." << std::endl;

    std::vector<std::string> lines_to_write = {"Line one from vector", "Line two from vector"};
    for(const std::string& line : lines_to_write) {
        outfile << line << std::endl;
    }

    std::cout << "Data written to " << FILENAME << std::endl;

    // 4. Close the file (automatically closed when outfile goes out of scope, but good practice to explicitly close)
    outfile.close();
    std::cout << FILENAME << " closed after writing." << std::endl;

    // Append to a file
    std::ofstream append_file(FILENAME, std::ios::app); // std::ios::app for append mode
    if (!append_file.is_open()) {
        std::cerr << "Error: Could not open file " << FILENAME << " for appending." << std::endl;
        return 1;
    }
    append_file << "This line is appended." << std::endl;
    append_file.close();
    std::cout << "Data appended to " << FILENAME << std::endl;


    // --- Reading from a File (std::ifstream) ---
    std::cout << "\n--- Reading from File ---" << std::endl;
    // 1. Open the file for reading
    std::ifstream infile(FILENAME); // Opens FILENAME for reading

    // 2. Check if the file was opened successfully
    if (!infile.is_open()) { // or !infile
        std::cerr << "Error: Could not open file " << FILENAME << " for reading." << std::endl;
        return 1; // Indicate an error
    }
    std::cout << FILENAME << " opened successfully for reading." << std::endl;

    std::string line;
    // 3. Read data from the file - Reading line by line
    std::cout << "\nContents of " << FILENAME << " (line by line):" << std::endl;
    while (std::getline(infile, line)) { // Reads until newline character
        std::cout << line << std::endl;
    }

    // Reset file stream state and position to read again
    infile.clear(); // Clear any error flags (like EOF)
    infile.seekg(0, std::ios::beg); // Seek to the beginning of the file

    // Reading word by word (or based on whitespace separation)
    std::cout << "\nContents of " << FILENAME << " (word by word):" << std::endl;
    std::string word;
    while (infile >> word) { // Reads until whitespace
        std::cout << word << std::endl;
    }

    // Reset again to demonstrate reading different types
    infile.clear();
    infile.seekg(0, std::ios::beg);

    std::cout << "\nReading specific data types from " << FILENAME << ":" << std::endl;
    std::string str_val;
    int int_val;
    double double_val;

    // Assuming first line is "Hello, File!"
    std::getline(infile, line); // Read "Hello, File!"
    std::cout << "Read line: " << line << std::endl;

    // Assuming second line is "This is the second line."
    std::getline(infile, line); // Read "This is the second line."
    std::cout << "Read line: " << line << std::endl;

    // Assuming third line is "42 is an integer."
    if (infile >> int_val >> str_val >> str_val) { // Reads 42, then "is", then "an"
         std::cout << "Read integer: " << int_val << std::endl;
    } else {
        std::cerr << "Error reading integer line." << std::endl;
    }
    std::getline(infile, line); // Consume rest of the line " integer."
                                // (infile >> str_val) would read "integer."

    // Assuming fourth line is "3.14159 is a double."
    if (infile >> double_val >> str_val >> str_val >> str_val) { // Reads 3.14159, then "is", "a", "double."
         std::cout << "Read double: " << double_val << std::endl;
    } else {
        std::cerr << "Error reading double line." << std::endl;
    }
    std::getline(infile, line); // Consume rest of the line (if any)


    // 4. Close the file (automatically closed when infile goes out of scope)
    infile.close();
    std::cout << "\n" << FILENAME << " closed after reading." << std::endl;

    std::cout << "\nFile I/O demonstration complete." << std::endl;

    return 0;
}
