// This program implements a simple CSV reader and writer.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream> // For std::stringstream in CSV reading

// --- CSV Writer ---

// Function to escape a string field for CSV:
// - If the field contains a comma, newline, or double quote, enclose it in double quotes.
// - Existing double quotes within the field are escaped by doubling them (e.g., " becomes "").
std::string escape_csv_field(const std::string& field) {
    bool needs_quoting = false;
    std::string escaped_field;
    escaped_field.reserve(field.length() + 2); // Reserve space, potentially more if many quotes

    for (char c : field) {
        if (c == ',' || c == '\n' || c == '"') {
            needs_quoting = true;
        }
        if (c == '"') {
            escaped_field += "\"\""; // Double the quote
        } else {
            escaped_field += c;
        }
    }

    if (needs_quoting) {
        return "\"" + escaped_field + "\"";
    }
    return field; // Return original if no special characters that require quoting (or only quotes that were escaped)
                  // If only quotes were present, they are already escaped, but quoting the whole field is safer.
                  // Strictly, only fields with comma, newline, or starting with quote *need* outer quotes.
                  // But common practice is to quote if any quote char appears, to be safe.
}


// Writes data to a CSV file.
// data_to_write: A vector of rows, where each row is a vector of strings (fields).
bool write_csv(const std::string& filename, const std::vector<std::vector<std::string>>& data_to_write) {
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }

    for (size_t i = 0; i < data_to_write.size(); ++i) {
        const auto& row = data_to_write[i];
        for (size_t j = 0; j < row.size(); ++j) {
            outfile << escape_csv_field(row[j]);
            if (j < row.size() - 1) {
                outfile << ",";
            }
        }
        // Only add newline if it's not the last row, or if you always want a trailing newline
        // if (i < data_to_write.size() - 1) { // Common practice
             outfile << "\n"; // Standard CSV ends each line with \n or \r\n
        // }
    }

    if (outfile.fail()) {
        std::cerr << "Error: An error occurred while writing to " << filename << std::endl;
        outfile.close();
        return false;
    }

    outfile.close();
    return true;
}

// --- CSV Reader ---

// Reads data from a CSV file.
// Returns a vector of rows, where each row is a vector of strings.
std::vector<std::vector<std::string>> read_csv(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return data; // Return empty data
    }

    std::string line;
    while (std::getline(infile, line)) {
        // Remove \r if present (for Windows EOL \r\n)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty() && infile.eof()) continue; // Skip empty line at EOF

        std::vector<std::string> row;
        std::stringstream line_stream(line);
        std::string field;
        char c;

        while (line_stream.get(c)) {
            if (line_stream.eof() && field.empty() && c == '\0') break; // Handle potential null char from stringstream get

            if (c == '"') { // Start of a quoted field
                field.clear(); // Clear any leading whitespace if not part of quote
                std::string quoted_content;
                bool in_quotes = true;
                while (in_quotes && line_stream.get(c)) {
                    if (c == '"') {
                        // Check for escaped quote: ""
                        if (line_stream.peek() == '"') {
                            quoted_content += '"'; // Add one quote
                            line_stream.get(c);   // Consume the second quote of the pair
                        } else {
                            in_quotes = false; // End of quoted field
                        }
                    } else {
                        quoted_content += c;
                    }
                }
                field = quoted_content;
                // After a quoted field, we expect a comma or end of line.
                // Consume the character immediately after the closing quote if it's not a comma.
                // This handles cases like "field"extra_chars,
                // The simple parser might not handle this robustly; proper CSV might disallow chars between quote and comma.
                // For this simple parser, we assume next char is comma or EOF.
                if(line_stream.peek() != EOF && line_stream.peek() != ',') {
                    // This indicates a potentially malformed CSV or a simple parser limitation
                    // We might want to read until the next comma or EOL.
                    // For now, let's assume the next char after quote is either comma or nothing.
                }

            } else { // Non-quoted field or part of it
                field.clear();
                field += c; // Add the first character
                while (line_stream.peek() != EOF && line_stream.peek() != ',') {
                    line_stream.get(c);
                    field += c;
                }
            }

            row.push_back(field);
            field.clear();

            if (line_stream.peek() == ',') {
                line_stream.get(c); // Consume the comma
            } else if (line_stream.peek() == EOF) {
                break; // End of line
            }
        }
         // If the line ends with a comma, it means there's an empty field at the end
        if (!line.empty() && line.back() == ',') {
            row.push_back("");
        }


        data.push_back(row);
    }

    if (infile.bad()) { // I/O error
        std::cerr << "Error: An I/O error occurred while reading " << filename << std::endl;
    }

    infile.close();
    return data;
}


int main() {
    const std::string output_filename = "sample.csv";

    // --- Demonstrate CSV Writer ---
    std::cout << "--- CSV Writer Demo ---" << std::endl;
    std::vector<std::vector<std::string>> data_to_write = {
        {"Name", "Age", "City", "Occupation"},
        {"Alice", "30", "New York", "Engineer, Software"},
        {"Bob", "24", "Los Angeles", "Artist"},
        {"Charlie", "35", "Chicago", "\"Project\" Manager"}, // Field with quotes
        {"David", "40", "Houston", "Contains\nNewline and, comma"} // Field with newline and comma
    };

    std::cout << "Writing data to " << output_filename << "..." << std::endl;
    if (write_csv(output_filename, data_to_write)) {
        std::cout << output_filename << " written successfully." << std::endl;
    } else {
        std::cout << "Failed to write " << output_filename << "." << std::endl;
        return 1; // Exit if write failed
    }

    // --- Demonstrate CSV Reader ---
    std::cout << "\n--- CSV Reader Demo ---" << std::endl;
    std::cout << "Reading data from " << output_filename << "..." << std::endl;
    std::vector<std::vector<std::string>> read_data = read_csv(output_filename);

    if (read_data.empty() && !data_to_write.empty()) { // Check if reading failed significantly
        std::cout << "Failed to read data from " << output_filename << " or file was empty." << std::endl;
    } else {
        std::cout << "Data read from " << output_filename << ":" << std::endl;
        for (const auto& row : read_data) {
            std::cout << "  [";
            for (size_t i = 0; i < row.size(); ++i) {
                std::cout << "\"" << row[i] << "\""; // Print field in quotes for clarity
                if (i < row.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
        }
    }

    // Test with a slightly more complex manually created CSV content
    const std::string test_csv_filename = "test_complex.csv";
    std::ofstream test_out(test_csv_filename);
    test_out << "Header1,Header2 with comma,Header3\"\"escaped\"\" quotes\n";
    test_out << "data1,\"data2, still data2\",\"data3 with \"\"\"\"quotes\"\"\"\"\n";
    test_out << "data4,,empty field before this\n"; // Field with nothing, then empty field
    test_out << "\"trailing comma test\",\n"; // Line with trailing comma
    test_out.close();

    std::cout << "\n--- CSV Reader Demo (Complex Test) ---" << std::endl;
    std::cout << "Reading data from " << test_csv_filename << "..." << std::endl;
    std::vector<std::vector<std::string>> complex_data = read_csv(test_csv_filename);
     if (complex_data.empty()) {
        std::cout << "Failed to read data from " << test_csv_filename << " or file was empty." << std::endl;
    } else {
        std::cout << "Data read from " << test_csv_filename << ":" << std::endl;
        for (const auto& row : complex_data) {
            std::cout << "  [";
            for (size_t i = 0; i < row.size(); ++i) {
                std::cout << "{\"" << row[i] << "\"}"; // Print field in quotes for clarity
                if (i < row.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
        }
    }


    std::cout << "\nSimple CSV Handler demonstration complete." << std::endl;
    return 0;
}
