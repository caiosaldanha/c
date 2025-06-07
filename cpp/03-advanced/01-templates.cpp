// This program demonstrates C++ templates, including function and class templates.
#include <iostream>
#include <string>
#include <vector>

// --- 1. Function Templates ---
// A function template allows you to define a generic function that can operate
// with different data types without rewriting the entire function for each type.

// Generic 'add' function template
template <typename T> // 'typename' or 'class' can be used here
T add(T a, T b) {
    return a + b;
}

// Generic 'printArray' function template
template <typename T, int Size> // Template with a non-type parameter (Size)
void printArray(T (&arr)[Size]) { // Takes a reference to an array of known size
    std::cout << "Array elements: [";
    for (int i = 0; i < Size; ++i) {
        std::cout << arr[i] << (i == Size - 1 ? "" : ", ");
    }
    std::cout << "]" << std::endl;
}

// Overload for std::vector (more common in modern C++)
template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "Vector elements: [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i == vec.size() - 1 ? "" : ", ");
    }
    std::cout << "]" << std::endl;
}

// --- 2. Class Templates ---
// A class template allows you to define a generic class that can be parameterized
// with types or values.

template <typename T>
class Container {
private:
    T element;
public:
    Container(T elem) : element(elem) {
        std::cout << "Container created with element: " << element << std::endl;
    }

    T getElement() const {
        return element;
    }

    void setElement(T elem) {
        element = elem;
        std::cout << "Container element set to: " << element << std::endl;
    }
};

// --- 3. Template Specialization (Optional but good to show) ---
// Template specialization allows you to provide a specific implementation
// for a template when used with a particular data type.

// Specialization for Container<std::string>
template <> // Indicates a full specialization
class Container<std::string> {
private:
    std::string element;
public:
    Container(const std::string& elem) : element(elem) {
        std::cout << "Specialized Container<std::string> created with element: \"" << element << "\"" << std::endl;
    }

    std::string getElement() const {
        return element;
    }

    void setElement(const std::string& elem) {
        element = elem;
        std::cout << "Specialized Container<std::string> element set to: \"" << element << "\"" << std::endl;
    }

    void printInfo() const {
        std::cout << "This is a specialized container for strings. Current value: \"" << element << "\"" << std::endl;
    }
};

// Function template specialization for add with const char* (to concatenate strings properly)
// Note: Overloading might be preferred over specialization for functions in many cases.
template <>
const char* add<const char*>(const char* a, const char* b) {
    // This is a simplified example; proper string concatenation would require dynamic memory.
    // For real C-string concatenation, it's better to use std::string or manage memory carefully.
    // This specialization is more for demonstrating syntax.
    // A better approach for string addition would be to use std::string with the generic template.
    std::string temp = std::string(a) + std::string(b);
    // The issue here is returning a pointer to a temporary string's internal buffer.
    // This is unsafe. For demonstration of specialization syntax only.
    // A safe way would require allocating memory that the caller must manage, or returning std::string.
    char* result = new char[temp.length() + 1];
    #ifdef _MSC_VER
        strcpy_s(result, temp.length() + 1, temp.c_str());
    #else
        strcpy(result, temp.c_str());
    #endif
    std::cout << "(Specialized 'add' for const char* called - NOTE: memory leak potential here for demo) ";
    return result; // Caller must delete[] this result.
}


int main() {
    std::cout << "--- Function Templates ---" << std::endl;
    // Integer addition
    std::cout << "add(5, 3): " << add(5, 3) << std::endl; // T is deduced as int
    // Double addition
    std::cout << "add(5.5, 3.3): " << add(5.5, 3.3) << std::endl; // T is deduced as double
    // String concatenation (std::string uses operator+ overload)
    std::cout << "add(std::string(\"Hello, \"), std::string(\"World!\")): "
              << add(std::string("Hello, "), std::string("World!")) << std::endl; // T is deduced as std::string

    // Using specialized add for const char*
    const char* c_str1 = "C-string ";
    const char* c_str2 = "concat";
    const char* c_result = add(c_str1, c_str2);
    std::cout << "add(\"" << c_str1 << "\", \"" << c_str2 << "\"): " << c_result << std::endl;
    delete[] c_result; // Clean up memory from specialized add

    // printArray
    int intArr[] = {1, 2, 3, 4, 5};
    double doubleArr[] = {1.1, 2.2, 3.3};
    std::string strArr[] = {"one", "two", "three"};

    printArray(intArr);
    printArray(doubleArr);
    printArray(strArr);

    // printVector
    std::vector<int> intVec = {10, 20, 30};
    std::vector<std::string> strVec = {"alpha", "beta", "gamma"};
    printVector(intVec);
    printVector(strVec);


    std::cout << "\n--- Class Templates ---" << std::endl;
    Container<int> intContainer(123);
    std::cout << "Element from intContainer: " << intContainer.getElement() << std::endl;
    intContainer.setElement(456);

    Container<double> doubleContainer(7.89);
    std::cout << "Element from doubleContainer: " << doubleContainer.getElement() << std::endl;

    // Using the specialized Container<std::string>
    Container<std::string> stringContainer("Test String");
    std::cout << "Element from stringContainer: " << stringContainer.getElement() << std::endl;
    stringContainer.setElement("Another Test");
    stringContainer.printInfo();


    std::cout << "\nTemplate demonstration complete." << std::endl;
    return 0;
}
