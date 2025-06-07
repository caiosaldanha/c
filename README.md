# C++ Learning Repository

This repository contains a collection of C++ code examples, organized by topic, to help beginners learn the language.

## Table of Contents

*   [Basics](cpp/01-basics) - Fundamental C++ concepts.
*   [Intermediate](cpp/02-intermediate/readme.md) - Covers STL containers, OOP, memory management, file I/O, and more. See the [Intermediate README](cpp/02-intermediate/readme.md) for a detailed list of examples.
*   [Advanced](cpp/03-advanced/readme.md) - Explores templates, STL algorithms, exception safety, RAII, move semantics, concurrency, and modern C++ features. See the [Advanced README](cpp/03-advanced/readme.md) for a detailed list.
*   [C++ in Data Science](cpp/04-data-science/readme.md) - Examples for numerical libraries (Eigen), custom data handlers (CSV), and performance considerations. See the [Data Science README](cpp/04-data-science/readme.md) for details.
*   [NVIDIA CUDA C++](cpp/05-cuda/readme.md) - Introductory examples to CUDA C++ for GPU programming. **Requires NVIDIA GPU & CUDA Toolkit.** See the [CUDA README](cpp/05-cuda/readme.md) for setup and details.

## Getting Started

### Prerequisites

*   A C++ compiler (e.g., g++, clang++) for general C++ examples.
*   NVIDIA CUDA Toolkit (including `nvcc`) and a CUDA-enabled NVIDIA GPU for CUDA examples.
*   make (optional, for using a Makefile if provided in specific examples).

### Compiling and Running Examples

Most general C++ examples in this repository are single `.cpp` files and can be compiled using a standard C++ compiler. For example:

```bash
g++ cpp/01-basics/01-hello-world.cpp -o hello_world
./hello_world
```

CUDA C++ examples (`.cu` files) must be compiled with `nvcc` (NVIDIA's CUDA compiler). For example:
```bash
nvcc cpp/05-cuda/01-cuda-device-query.cu -o device_query
./device_query
```
Always refer to the `readme.md` file within each specific section (e.g., `cpp/05-cuda/readme.md`) for detailed prerequisites and compilation instructions relevant to those examples.

For more complex examples or projects, a `Makefile` might be provided. In such cases, navigate to the example's directory and run `make`.

## Coding Style

(To be defined)

## Contribution Guidelines

(To be defined)
