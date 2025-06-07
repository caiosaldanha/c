# NVIDIA CUDA C++ Examples

This section provides introductory examples to programming with NVIDIA CUDA C++, focusing on leveraging the parallel processing capabilities of NVIDIA GPUs.

## Prerequisites

To compile and run these examples, you **must** have the following setup:
1.  **NVIDIA GPU**: An NVIDIA graphics card that is CUDA-enabled. Most modern NVIDIA GeForce, Quadro, and Tesla GPUs support CUDA.
2.  **NVIDIA CUDA Toolkit**: This toolkit includes the `nvcc` compiler, CUDA libraries (cuBLAS, cuFFT, etc.), and runtime components. You can download it from the [NVIDIA CUDA Toolkit website](https://developer.nvidia.com/cuda-downloads). Ensure the version is compatible with your GPU and driver.
3.  **NVIDIA GPU Driver**: A compatible NVIDIA display driver that supports your CUDA Toolkit version. Drivers are often updated with new toolkit releases.

Without these components, you will not be able to compile or run the CUDA examples.

## Compilation

CUDA C++ files (typically with a `.cu` extension) are compiled using the NVIDIA CUDA Compiler (`nvcc`), which is part of the CUDA Toolkit.

A typical compilation command for the examples in this section is:
```bash
nvcc your_file.cu -o output_executable_name
```
For example, to compile and run the `01-cuda-device-query.cu` example:
```bash
nvcc 01-cuda-device-query.cu -o device_query
./device_query
```
Some examples might require linking against specific CUDA libraries if they use them (e.g., `-lcublas` for cuBLAS), but the current examples are self-contained for basic runtime usage. Always check the comments within individual `.cu` files for any specific compilation notes.

## Examples

1.  [CUDA Device Query](./01-cuda-device-query.cu) - This program queries and displays detailed information about all CUDA-enabled GPUs detected on your system. It's an excellent first step to verify your CUDA installation and see your GPU's capabilities.
2.  [Vector Addition on GPU](./02-vector-add.cu) - A fundamental CUDA example that implements vector addition (`C = A + B`) on the GPU. It demonstrates key concepts such as:
    *   Defining a `__global__` kernel function to run on the GPU.
    *   Allocating and managing memory on the GPU (device memory).
    *   Transferring data between the host (CPU) and the device (GPU).
    *   Configuring and launching a kernel with a specific grid and block structure.
    *   Basic error checking for CUDA API calls.

---

Refer to the main [repository README](../../README.md) for general C++ project information and compilation instructions for non-CUDA C++ files.
