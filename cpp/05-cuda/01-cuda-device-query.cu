// This program queries and displays information about CUDA-enabled GPUs on the system.
//
// ================================= IMPORTANT ==================================
// TO COMPILE AND RUN THIS FILE:
// 1. CUDA Toolkit: You must have the NVIDIA CUDA Toolkit installed.
//    Download from https://developer.nvidia.com/cuda-downloads
// 2. NVIDIA GPU: You need an NVIDIA CUDA-enabled GPU.
// 3. NVIDIA Driver: Ensure you have a compatible NVIDIA driver installed.
// 4. Compiler: Use the NVIDIA CUDA Compiler (nvcc).
//
// COMPILE COMMAND (example):
//   nvcc 01-cuda-device-query.cu -o 01-cuda-device-query
//
// RUN COMMAND (example):
//   ./01-cuda-device-query
// ==============================================================================

#include <cuda_runtime.h> // CUDA Runtime API
#include <iostream>       // For std::cout, std::cerr
#include <cstdio>         // For printf (can be more convenient for formatting)

// --- CUDA Error Checking Macro ---
// This simple macro wraps CUDA API calls and checks for errors.
// More robust error handling might involve a function.
#define CUDA_CHECK(err) __cuda_check_errors((err), __FILE__, __LINE__)

inline void __cuda_check_errors(cudaError_t err, const char *file, int line) {
    if (cudaSuccess != err) {
        fprintf(stderr, "CUDA error: %s (%d) in %s at line %d\n",
                cudaGetErrorString(err), err, file, line);
        // It's common to exit or throw an exception upon CUDA error.
        // For this example, we'll just print and let it continue if possible,
        // or exit if critical (like no devices).
    }
}


int main() {
    std::cout << "--- CUDA Device Query ---" << std::endl;

    int deviceCount = 0;
    cudaError_t error_id = cudaGetDeviceCount(&deviceCount);

    if (error_id != cudaSuccess) {
        fprintf(stderr, "cudaGetDeviceCount failed! CUDA driver and runtime version may be mismatched.\n");
        fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(error_id));
        if (error_id == cudaErrorInsufficientDriver) {
             fprintf(stderr, "Ensure you have a recent NVIDIA driver installed.\n");
        }
        return 1; // Critical error, cannot proceed
    }

    CUDA_CHECK(error_id); // Check for other potential errors from cudaGetDeviceCount

    if (deviceCount == 0) {
        std::cout << "No CUDA-enabled devices found on this system." << std::endl;
        return 0; // Not an error, but nothing to query
    }

    std::cout << "Found " << deviceCount << " CUDA-enabled device(s)." << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;

    // Loop through each device
    for (int dev = 0; dev < deviceCount; ++dev) {
        cudaDeviceProp deviceProp; // Structure to hold device properties
        CUDA_CHECK(cudaGetDeviceProperties(&deviceProp, dev)); // Get properties for device 'dev'

        printf("Device %d: \"%s\"\n", dev, deviceProp.name);

        // Compute Capability (major.minor)
        // This defines the features supported by the GPU hardware.
        // Higher numbers generally mean more features and better performance.
        printf("  Compute Capability:        %d.%d\n", deviceProp.major, deviceProp.minor);

        // Total Global Memory
        // This is the total amount of memory available on the GPU's dedicated RAM (e.g., GDDR6).
        printf("  Total Global Memory:       %.0f MiB (%llu bytes)\n",
               static_cast<float>(deviceProp.totalGlobalMem) / (1024.0f * 1024.0f),
               (unsigned long long)deviceProp.totalGlobalMem);

        // Shared Memory Per Block
        // Fast on-chip memory shared by threads within a block. Crucial for performance.
        printf("  Shared Memory Per Block:   %zu bytes\n", deviceProp.sharedMemPerBlock);

        // Registers Per Block
        printf("  Registers Per Block:       %d\n", deviceProp.regsPerBlock);

        // Warp Size
        // Threads on a CUDA GPU execute in groups called warps. This is typically 32.
        printf("  Warp Size:                 %d threads\n", deviceProp.warpSize);

        // Max Threads Per Block
        // The maximum number of threads that can be launched in a single execution block.
        printf("  Max Threads Per Block:     %d\n", deviceProp.maxThreadsPerBlock);

        // Max Threads Per Multiprocessor
        printf("  Max Threads Per SM:        %d\n", deviceProp.maxThreadsPerMultiProcessor);

        // Max Dimensions of a Block (x, y, z)
        // Threads in a block can be organized in 1D, 2D, or 3D. These are the max sizes.
        printf("  Max Block Dimensions:      %d x %d x %d\n",
               deviceProp.maxThreadsDim[0], deviceProp.maxThreadsDim[1], deviceProp.maxThreadsDim[2]);

        // Max Dimensions of a Grid (x, y, z)
        // Blocks are organized into a grid. These are the max dimensions of the grid.
        printf("  Max Grid Dimensions:       %d x %d x %d\n",
               deviceProp.maxGridSize[0], deviceProp.maxGridSize[1], deviceProp.maxGridSize[2]);

        // Clock Rate
        printf("  GPU Clock Rate:            %.0f MHz (%0.2f GHz)\n",
                deviceProp.clockRate * 1e-3f, deviceProp.clockRate * 1e-6f);

        // Memory Clock Rate
        printf("  Memory Clock Rate:         %.0f MHz\n", deviceProp.memoryClockRate * 1e-3f);

        // Memory Bus Width
        printf("  Memory Bus Width:          %d-bit\n", deviceProp.memoryBusWidth);

        // L2 Cache Size
        if (deviceProp.l2CacheSize > 0) {
            printf("  L2 Cache Size:             %d bytes\n", deviceProp.l2CacheSize);
        }

        // Number of Multiprocessors (SMs)
        printf("  Number of Multiprocessors: %d\n", deviceProp.multiProcessorCount);

        // CUDA Cores (approximate, depends on architecture)
        // This calculation is a common heuristic but not officially defined by NVIDIA as "CUDA Cores" in deviceProp.
        // It's derived from SM count and cores per SM for different architectures.
        // For Kepler: 192 cores/SM. Maxwell: 128. Pascal: 64 or 128. Volta/Turing/Ampere: 64 FP32 cores/SM.
        int cudaCores = 0;
        if (deviceProp.major == 2) { // Fermi
            if (deviceProp.minor == 1) cudaCores = deviceProp.multiProcessorCount * 48;
            else cudaCores = deviceProp.multiProcessorCount * 32;
        } else if (deviceProp.major == 3) { // Kepler
            cudaCores = deviceProp.multiProcessorCount * 192;
        } else if (deviceProp.major == 5) { // Maxwell
            cudaCores = deviceProp.multiProcessorCount * 128;
        } else if (deviceProp.major == 6) { // Pascal
            if (deviceProp.minor == 0 || deviceProp.minor == 1) cudaCores = deviceProp.multiProcessorCount * 64; // GP100, GP104 might have 64 or 128 (e.g. P100 vs 1080)
            else cudaCores = deviceProp.multiProcessorCount * 128; // GP102, GP104, etc often 128
        } else if (deviceProp.major == 7) { // Volta / Turing
            cudaCores = deviceProp.multiProcessorCount * 64;
        } else if (deviceProp.major == 8) { // Ampere
            cudaCores = deviceProp.multiProcessorCount * (deviceProp.minor == 9 ? 64 : 128); // A100 has 64 FP32/SM, GA10x gaming GPUs have 128 FP32 units/SM in a different config
        } else if (deviceProp.major == 9) { // Hopper
             cudaCores = deviceProp.multiProcessorCount * 128; // H100 has 128 FP32 CUDA cores per SM
        } else {
            cudaCores = deviceProp.multiProcessorCount; // Fallback or unknown
        }
        if (cudaCores > 0) {
            printf("  Approximate CUDA Cores:    %d\n", cudaCores);
        }

        printf("  Concurrent Kernels:        %s\n", deviceProp.concurrentKernels ? "Yes" : "No");
        printf("  ECC Enabled:               %s\n", deviceProp.ECCEnabled ? "Yes" : "No");
        printf("------------------------------------------------------\n");
    }

    // Reset the device to free resources associated with this context (optional for simple query)
    // CUDA_CHECK(cudaDeviceReset());

    return 0;
}
