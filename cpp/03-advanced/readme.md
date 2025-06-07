# Advanced C++ Examples

This section delves into more complex and nuanced C++ topics, essential for writing robust, efficient, and modern C++ code.

1.  [Templates (Function and Class)](./01-templates.cpp) - Explores generic programming with function templates (e.g., generic `add`, `printArray`) and class templates (e.g., generic `Container`), including basic template specialization.
2.  [Advanced STL - Algorithms](./02a-stl-algorithms.cpp) - Demonstrates common STL algorithms such as `for_each`, `find`, `count_if`, `transform`, `copy`, `remove_if` (with erase-remove idiom), and `sort`, often using lambda expressions.
3.  [Advanced STL - Other Containers](./02b-stl-other-containers.cpp) - Briefly introduces `std::list`, `std::deque`, and `std::priority_queue`, highlighting their characteristics and basic operations.
4.  [Exception Safety Guarantees](./03-exception-safety.cpp) - Discusses and illustrates no-safety, basic guarantee, and strong exception safety guarantee (commit-or-rollback) with examples.
5.  [RAII Beyond Pointers](./04-raii-beyond-pointers.cpp) - Shows Resource Acquisition Is Initialization (RAII) applied to resources like file handles (`FileHandler`) and mutex locks (`MutexGuard`), ensuring proper cleanup.
6.  [Move Semantics and Rvalue References](./05-move-semantics-rvalues.cpp) - Covers rvalue references, move constructors, move assignment operators (`std::move`), and perfect forwarding, crucial for performance optimization.
7.  [Concurrency Basics](./06-concurrency-basics.cpp) - Introduces C++ concurrency with `std::thread`, `std::mutex`, `std::lock_guard`, `std::unique_lock`, `std::async`, `std::future`, `std::promise`, and `std::atomic`.
8.  [Modern C++ Features](./07-modern-cpp-features.cpp) - Showcases a selection of modern C++ features including advanced lambdas (generic, `this` capture, mutable), `auto` and `decltype`, `constexpr` functions, and `std::optional` (C++17).

Refer to the main [repository README](../../README.md) for compilation instructions and overall project structure.
