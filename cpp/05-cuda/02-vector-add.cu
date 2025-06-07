// This program implements vector addition on the GPU using CUDA.
// It demonstrates basic CUDA concepts: kernel definition, memory management,
// data transfers between host and device, and kernel launch.
//
// ================================= IMPORTANT ==================================
// TO COMPILE AND RUN THIS FILE:
// 1. CUDA Toolkit: You must have the NVIDIA CUDA Toolkit installed.
// 2. NVIDIA GPU: You need an NVIDIA CUDA-enabled GPU.
// 3. NVIDIA Driver: Ensure you have a compatible NVIDIA driver installed.
// 4. Compiler: Use the NVIDIA CUDA Compiler (nvcc).
//
// COMPILE COMMAND (example):
//   nvcc 02-vector-add.cu -o 02-vector-add
//
// RUN COMMAND (example):
//   ./02-vector-add
// ==============================================================================

#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <cmath> // For fabs in verification
#include <iomanip> // For std::fixed, std::setprecision

// --- CUDA Error Checking Macro ---
#define CUDA_CHECK(err) __cuda_check_errors((err), __FILE__, __LINE__)

inline void __cuda_check_errors(cudaError_t err, const char *file, int line) {
    if (cudaSuccess != err) {
        fprintf(stderr, "CUDA error: %s (%d) in %s at line %d\n",
                cudaGetErrorString(err), err, file, line);
        // For this example, exit on any CUDA error after the initial setup.
        exit(EXIT_FAILURE);
    }
}

// --- Vector Addition Kernel ---
// This function runs on the GPU (device code).
// `__global__` indicates it's a kernel callable from the host.
// It performs element-wise addition: c[i] = a[i] + b[i].
__global__ void vectorAddKernel(const float *a, const float *b, float *c, int n) {
    // Calculate the global thread ID for this thread.
    // - blockIdx.x: The ID of the current block in the grid (0-indexed).
    // - blockDim.x: The number of threads in each block (specified at launch).
    // - threadIdx.x: The ID of the current thread within its block (0-indexed).
    int threadID = blockIdx.x * blockDim.x + threadIdx.x;

    // Check array bounds to ensure we don't write past the end of the arrays.
    // This is important if n is not perfectly divisible by threadsPerBlock.
    if (threadID < n) {
        c[threadID] = a[threadID] + b[threadID];
    }
}


int main() {
    std::cout << "--- CUDA Vector Addition ---" << std::endl;

    // --- 1. Initialization ---
    const int n = 1024 * 1024; // Number of elements in the vectors (e.g., 2^20)
    const size_t bytes = n * sizeof(float);
    std::cout << "Vector size: " << n << " elements (" << bytes / (1024.0 * 1024.0) << " MiB per vector)" << std::endl;

    // Allocate host memory (RAM) for vectors using std::vector for automatic memory management.
    std::vector<float> h_a(n);
    std::vector<float> h_b(n);
    std::vector<float> h_c(n); // Host vector to store the result from the device

    // Initialize host vectors h_a and h_b
    for (int i = 0; i < n; ++i) {
        h_a[i] = static_cast<float>(i) * 0.5f + 1.0f; // Example values
        h_b[i] = static_cast<float>(n - i) * 0.25f + 0.5f; // Example values
    }
    std::cout << "Host vectors initialized." << std::endl;

    // --- 2. Device Memory Allocation ---
    // Pointers for device memory (GPU VRAM)
    float *d_a = nullptr;
    float *d_b = nullptr;
    float *d_c = nullptr;

    // Allocate memory on the GPU device
    CUDA_CHECK(cudaMalloc((void **)&d_a, bytes));
    CUDA_CHECK(cudaMalloc((void **)&d_b, bytes));
    CUDA_CHECK(cudaMalloc((void **)&d_c, bytes));
    std::cout << "Device memory allocated." << std::endl;

    // --- 3. Data Transfer (Host to Device) ---
    // Copy data from host vectors (h_a, h_b) to device vectors (d_a, d_b).
    // cudaMemcpyHostToDevice: Transfer data from RAM to GPU VRAM.
    CUDA_CHECK(cudaMemcpy(d_a, h_a.data(), bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_b, h_b.data(), bytes, cudaMemcpyHostToDevice));
    std::cout << "Data transferred from host to device." << std::endl;

    // --- 4. Kernel Launch Configuration ---
    // Define the number of threads per block.
    // This is often chosen based on GPU architecture (e.g., multiples of warp size, typically 32).
    // Common values are 128, 256, 512, 1024. Max is usually 1024.
    const int threadsPerBlock = 256;

    // Calculate the number of blocks in the grid.
    // This ensures enough threads are launched to cover all 'n' elements.
    // The formula (n + threadsPerBlock - 1) / threadsPerBlock is a common idiom for ceiling division.
    const int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    std::cout << "Kernel launch configuration: " << std::endl;
    std::cout << "  Threads per block: " << threadsPerBlock << std::endl;
    std::cout << "  Blocks per grid:   " << blocksPerGrid << std::endl;
    std::cout << "  Total threads:     " << threadsPerBlock * blocksPerGrid << " (may be > n)" << std::endl;


    // --- 5. Kernel Launch ---
    // Launch the vectorAddKernel on the GPU.
    // Syntax: kernel_name<<<blocksPerGrid, threadsPerBlock>>>(argument_list);
    std::cout << "Launching vectorAddKernel..." << std::endl;
    vectorAddKernel<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, n);

    // Check for errors during kernel launch (asynchronous, so errors might appear later)
    CUDA_CHECK(cudaGetLastError());
    // For robust synchronization and error checking after kernel, use cudaDeviceSynchronize.
    // This waits for all preceding device operations (including kernel) to complete.
    CUDA_CHECK(cudaDeviceSynchronize());
    std::cout << "Kernel execution finished." << std::endl;


    // --- 6. Data Transfer (Device to Host) ---
    // Copy the result vector (d_c) from device memory back to host memory (h_c).
    // cudaMemcpyDeviceToHost: Transfer data from GPU VRAM to RAM.
    CUDA_CHECK(cudaMemcpy(h_c.data(), d_c, bytes, cudaMemcpyDeviceToHost));
    std::cout << "Result transferred from device to host." << std::endl;

    // --- 7. Verification (Optional but Recommended) ---
    std::cout << "Verifying result on the host..." << std::endl;
    bool success = true;
    float epsilon = 1e-5f; // Tolerance for floating point comparisons
    int errors_found = 0;
    int max_errors_to_print = 10;

    for (int i = 0; i < n; ++i) {
        float expected = h_a[i] + h_b[i];
        if (std::fabs(h_c[i] - expected) > epsilon) {
            success = false;
            if (errors_found < max_errors_to_print) {
                std::cerr << std::fixed << std::setprecision(6)
                          << "Verification FAILED at index " << i << ": "
                          << "Host_A=" << h_a[i] << ", Host_B=" << h_b[i]
                          << ", Expected=" << expected << ", GPU_Result=" << h_c[i]
                          << ", Diff=" << std::fabs(h_c[i] - expected) << std::endl;
            }
            errors_found++;
        }
    }

    if (success) {
        std::cout << "Verification PASSED! All elements match." << std::endl;
    } else {
        std::cout << "Verification FAILED. " << errors_found << " mismatch(es) found." << std::endl;
        if (errors_found > max_errors_to_print) {
            std::cout << "(Only first " << max_errors_to_print << " mismatches shown)" << std::endl;
        }
    }

    // --- 8. Cleanup ---
    // Free GPU device memory
    CUDA_CHECK(cudaFree(d_a));
    CUDA_CHECK(cudaFree(d_b));
    CUDA_CHECK(cudaFree(d_c));
    std::cout << "Device memory freed." << std::endl;

    // Host memory (h_a, h_b, h_c) is managed by std::vector and will be freed automatically.

    std::cout << "\nCUDA vector addition demonstration complete." << std::endl;

    // Optional: Reset CUDA device to clean up context resources, not strictly necessary here.
    // CUDA_CHECK(cudaDeviceReset());

    return 0;
}
