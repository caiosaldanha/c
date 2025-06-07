// This program demonstrates a computationally intensive task in C++ (dot product)
// and discusses its relevance to data science performance.
#include <iostream>
#include <vector>
#include <numeric>      // For std::inner_product (for comparison, though we'll implement manually)
#include <stdexcept>    // For std::invalid_argument
#include <chrono>       // For timing
#include <iomanip>      // For std::fixed, std::setprecision

// --- Core Logic: Dot Product Calculation ---
// Calculates the dot product of two vectors.
// The dot product is the sum of the products of corresponding elements.
double calculate_dot_product(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    // Ensure vector sizes match
    if (vec1.size() != vec2.size()) {
        throw std::invalid_argument("Vector sizes must match for dot product calculation.");
    }
    if (vec1.empty()) {
        return 0.0; // Dot product of empty vectors is typically 0
    }

    double result = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}


int main() {
    std::cout << "--- C++ Performance Demo for Data Science ---" << std::endl;

    // Create two large vectors
    size_t vector_size = 1000000; // 1 million elements
    std::vector<double> v1(vector_size);
    std::vector<double> v2(vector_size);

    // Initialize with some values
    // For simplicity, v1[i] = i * 0.1, v2[i] = (vector_size - i) * 0.01
    for (size_t i = 0; i < vector_size; ++i) {
        v1[i] = static_cast<double>(i) * 0.1;
        v2[i] = static_cast<double>(vector_size - i) * 0.01;
    }
    std::cout << "Initialized two vectors of size " << vector_size << "." << std::endl;

    // --- Time the calculate_dot_product function ---
    std::cout << "\nCalculating dot product manually..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    double dot_product_result = 0.0;
    try {
        dot_product_result = calculate_dot_product(v1, v2);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_ms = end_time - start_time;

    std::cout << std::fixed << std::setprecision(4); // Format output
    std::cout << "Manual Dot Product Result: " << dot_product_result << std::endl;
    std::cout << "Time taken by calculate_dot_product: " << duration_ms.count() << " ms" << std::endl;

    // For comparison, std::inner_product (often highly optimized)
    // std::cout << "\nCalculating dot product using std::inner_product (for comparison)..." << std::endl;
    // start_time = std::chrono::high_resolution_clock::now();
    // double stl_dot_product_result = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
    // end_time = std::chrono::high_resolution_clock::now();
    // duration_ms = end_time - start_time;
    // std::cout << "STL inner_product Result: " << stl_dot_product_result << std::endl;
    // std::cout << "Time taken by std::inner_product: " << duration_ms.count() << " ms" << std::endl;


    // --- Comments for Discussion ---

    // 1. Why C++ can be significantly faster for numerical computations:
    //    - Compilation to Machine Code: C++ is a compiled language. The source code is translated
    //      directly into machine code that the CPU can execute. This avoids the overhead of
    //      interpretation that languages like Python (with CPython interpreter) have for each line of code.
    //    - Static Typing: Types are checked at compile time. This allows the compiler to make
    //      many optimizations because it knows the exact data types and memory layouts.
    //      Dynamic typing in languages like Python requires type checking at runtime, adding overhead.
    //    - Manual Memory Management (Optional but Relevant): While modern C++ encourages RAII and smart
    //      pointers, the ability to manage memory directly (e.g., using `new`/`delete` or custom allocators)
    //      can be crucial for performance-critical sections, allowing for optimized memory access patterns
    //      and reducing overhead from garbage collection (which Python has).
    //    - Low-level Access: C++ provides closer access to hardware and memory, allowing for fine-tuned
    //      optimizations (e.g., SIMD instructions, memory alignment) that are harder to achieve in
    //      higher-level languages.
    //    - Optimized Libraries: Core numerical libraries in C++ (like Eigen, BLAS, LAPACK implementations)
    //      are often highly optimized, sometimes using assembly language for critical routines.
    //    - No Global Interpreter Lock (GIL) for true parallelism (in Python, CPython's GIL limits CPU-bound parallelism for threads).
    //      C++ threads can run truly in parallel on multi-core processors for CPU-bound tasks.

    // 2. Common ways C++ code is integrated into data science workflows:
    //    - Creating Shared Libraries (.so on Linux, .dll on Windows):
    //      The C++ code (like our `calculate_dot_product` or more complex algorithms) can be compiled
    //      into a shared library. This library exposes a C-compatible API (using `extern "C"`) or a
    //      C++ API that can then be called from Python.
    //      Tools for Python integration:
    //        - `ctypes`: A Python standard library module to call functions in shared libraries directly from Python.
    //          Requires a C API from the C++ library.
    //        - `cffi` (C Foreign Function Interface for Python): Another way to call C code, often easier
    //          than `ctypes` for more complex C APIs.
    //        - `Cython`: Allows writing C-extensions for Python. You can write Cython code that wraps C++
    //          code and provides a Pythonic interface. It's a mix of Python and C syntax.
    //        - `pybind11`: A popular header-only C++11 library for creating Python bindings for existing
    //          C++ code. It's known for its ease of use and ability to handle complex C++ types seamlessly.
    //        - `SWIG` (Simplified Wrapper and Interface Generator): A mature tool that can generate bindings
    //          for various scripting languages (including Python) from C/C++ header files.
    //      Example Workflow:
    //      1. Write performance-critical module in C++.
    //      2. Compile it into a shared library.
    //      3. Write Python code that loads this library and calls the C++ functions for heavy lifting,
    //         while using Python for higher-level logic, data manipulation (e.g., with Pandas), and plotting.

    //    - Standalone Command-Line Tools:
    //      C++ can be used to create fast command-line executables that process large datasets, perform
    //      simulations, or run complex algorithms. These tools can be part of a larger data pipeline,
    //      potentially orchestrated by shell scripts or workflow management tools (e.g., Snakemake, Nextflow).
    //      Data can be passed via files (CSV, binary formats) or standard input/output streams.

    //    - Using C++ Libraries Directly (e.g., within a C++ data science project):
    //      Sometimes, the entire data analysis pipeline or application is built in C++, especially when
    //      performance is paramount or when integrating with existing C++ systems (e.g., in finance,
    //      game development, scientific computing). Libraries like Eigen, dlib, MLPACK, TensorFlow C++ API,
    //      and OpenCV are used.

    // 3. Relevance of Libraries like Eigen:
    //    While our manual `calculate_dot_product` demonstrates C++ execution speed, for serious numerical
    //    work, dedicated libraries like Eigen are preferred.
    //    - Optimization: Eigen uses template metaprogramming and expression templates to generate highly
    //      optimized code for linear algebra operations, often outperforming manual loops. It can utilize
    //      SIMD (Single Instruction, Multiple Data) vectorization for parallel processing of data.
    //    - API: Provides a clean, expressive, and MATLAB-like API for matrix and vector manipulations.
    //    - Functionality: Offers a vast range of operations beyond basic arithmetic, including decompositions
    //      (LU, QR, SVD), solvers for linear systems, eigenvalue computations, etc.
    //    Using Eigen for the dot product would typically be `double eigen_dot = v1_eigen.dot(v2_eigen);`
    //    where v1_eigen and v2_eigen are Eigen::VectorXd types. This is usually faster and less error-prone.

    // 4. Trade-offs: Development Speed vs. Execution Speed:
    //    - Python: Generally offers faster development and prototyping due to its dynamic nature, extensive
    //      high-level libraries (Pandas, NumPy, Scikit-learn), and simpler syntax. Excellent for data
    //      exploration, visualization, and building many types of models quickly.
    //    - C++: Typically has a slower development cycle due to manual memory management (historically),
    //      stricter type system, and compilation times. However, it offers superior execution speed and
    //      control for performance-critical components.
    //    The "best of both worlds" approach is common: prototype in Python, identify bottlenecks, and then
    //    optimize those specific parts by rewriting them in C++ and integrating them back into the Python workflow.

    std::cout << "\n--- End of Discussion Comments ---" << std::endl;
    std::cout << "This example illustrated C++ performance for a simple numerical task." << std::endl;
    std::cout << "In real-world data science, C++ is often used for performance-critical modules "
              << "or libraries, integrated with higher-level languages like Python." << std::endl;

    return 0;
}
