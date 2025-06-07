// This program demonstrates common STL algorithms from the <algorithm> header.
#include <iostream>
#include <vector>
#include <list> // For remove_if example
#include <algorithm> // Required for STL algorithms
#include <numeric>   // For std::iota (useful for initializing)
#include <string>

// Helper function to print a vector
template<typename T>
void print_vector(const std::string& prefix, const std::vector<T>& vec) {
    std::cout << prefix;
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 1); // Fill vector with 1, 2, ..., 10
    print_vector("Initial vector v: ", v);

    // --- 1. Non-modifying sequence operations ---
    std::cout << "\n--- Non-modifying sequence operations ---" << std::endl;

    // a) std::for_each: Applies a function to each element in a range.
    std::cout << "std::for_each (print elements multiplied by 2): ";
    std::for_each(v.begin(), v.end(), [](int x) { // Lambda function
        std::cout << x * 2 << " ";
    });
    std::cout << std::endl;

    // b) std::find: Finds the first occurrence of a value in a range.
    int value_to_find = 5;
    auto it_find = std::find(v.begin(), v.end(), value_to_find);
    if (it_find != v.end()) {
        std::cout << "std::find: Value " << value_to_find << " found at index " << std::distance(v.begin(), it_find) << std::endl;
    } else {
        std::cout << "std::find: Value " << value_to_find << " not found." << std::endl;
    }

    // c) std::count_if: Counts elements satisfying a condition.
    int even_count = std::count_if(v.begin(), v.end(), [](int x) { // Lambda as predicate
        return x % 2 == 0;
    });
    std::cout << "std::count_if: Number of even elements: " << even_count << std::endl;

    // --- 2. Modifying sequence operations ---
    std::cout << "\n--- Modifying sequence operations ---" << std::endl;

    // a) std::transform: Applies an operation to each element and stores the result in a destination range.
    std::vector<int> v_squared(v.size());
    std::transform(v.begin(), v.end(), v_squared.begin(), [](int x) { // Unary operation
        return x * x;
    });
    print_vector("std::transform (squares of v): ", v_squared);

    std::vector<int> v_sum(v.size());
    std::transform(v.begin(), v.end(), v_squared.begin(), v_sum.begin(), [](int x, int y){ // Binary operation
        return x + y; // v[i] + v_squared[i]
    });
    print_vector("std::transform (v + v_squared): ", v_sum);


    // b) std::copy: Copies elements from one range to another.
    std::vector<int> v_copy(v.size());
    std::copy(v.begin(), v.end(), v_copy.begin());
    print_vector("std::copy (copy of v): ", v_copy);

    // std::copy_if
    std::vector<int> v_even_elements; // Destination vector can be different size
    std::copy_if(v.begin(), v.end(), std::back_inserter(v_even_elements), [](int x){
        return x % 2 == 0;
    });
    print_vector("std::copy_if (even elements from v): ", v_even_elements);


    // c) std::remove_if (with the erase-remove idiom for vectors)
    // std::remove_if doesn't actually remove elements from the container (for vectors).
    // It shunts the elements to be kept to the beginning of the range and returns
    // an iterator to the new 'logical' end of the range.
    // The container's size remains unchanged. The erase-remove idiom is then used to
    // actually remove the elements.
    std::vector<int> v_remove = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_vector("Before std::remove_if (v_remove): ", v_remove);

    auto new_end = std::remove_if(v_remove.begin(), v_remove.end(), [](int x) {
        return x % 3 == 0; // Remove elements divisible by 3
    });

    print_vector("After std::remove_if (logical removal, v_remove): ", v_remove); // Note size is still same, trailing elements are unspecified
    std::cout << "Logical size after remove_if: " << std::distance(v_remove.begin(), new_end) << std::endl;

    // Erase-remove idiom: actually remove the elements
    v_remove.erase(new_end, v_remove.end());
    print_vector("After v_remove.erase (actual removal): ", v_remove);

    // For std::list, remove_if is a member function and behaves as expected (actually removes elements)
    std::list<int> l_remove = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    l_remove.remove_if([](int x){ return x % 3 == 0; }); // Member function
    std::cout << "std::list after l_remove.remove_if: ";
    for(int x : l_remove) std::cout << x << " ";
    std::cout << std::endl;


    // --- 3. Sorting operations ---
    std::cout << "\n--- Sorting operations ---" << std::endl;
    std::vector<int> v_sort = {5, 1, 4, 2, 8, 0, 7, 3, 6, 9};
    print_vector("Before std::sort (v_sort): ", v_sort);

    // Sort in ascending order
    std::sort(v_sort.begin(), v_sort.end());
    print_vector("After std::sort (ascending): ", v_sort);

    // Sort in descending order using a lambda as a custom comparison function
    std::sort(v_sort.begin(), v_sort.end(), [](int a, int b) {
        return a > b; // True if 'a' should come before 'b'
    });
    print_vector("After std::sort (descending with lambda): ", v_sort);

    // Sorting with std::greater
    std::sort(v_sort.begin(), v_sort.end(), std::greater<int>()); // Using std::greater
    print_vector("After std::sort (descending with std::greater): ", v_sort);


    // --- Lambda expressions in more detail ---
    std::cout << "\n--- Lambda expressions ---" << std::endl;
    // Lambda syntax: [capture_clause](parameters) -> return_type { body }
    // - capture_clause: [], [&], [=], [this], [a, &b]
    // - parameters: optional, like function parameters
    // - -> return_type: optional, often deduced by compiler
    // - body: function body

    int factor = 10;
    std::vector<int> v_lambda_demo = {1, 2, 3};

    // Lambda capturing 'factor' by value
    std::cout << "Lambda capture by value: ";
    std::for_each(v_lambda_demo.begin(), v_lambda_demo.end(), [factor](int x) {
        std::cout << x * factor << " ";
    });
    std::cout << std::endl;

    // Lambda capturing 'factor' by reference
    std::cout << "Lambda capture by reference (factor before modify = " << factor << "): ";
    std::for_each(v_lambda_demo.begin(), v_lambda_demo.end(), [&factor](int& x) { // x is also by reference here
        x *= factor; // Modify elements of v_lambda_demo
        factor++;    // Modify captured 'factor'
    });
    print_vector("\nVector after modification by lambda: ", v_lambda_demo);
    std::cout << "Factor after modification by lambda: " << factor << std::endl;

    std::cout << "\nSTL algorithms demonstration complete." << std::endl;
    return 0;
}
