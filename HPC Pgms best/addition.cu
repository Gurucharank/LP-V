#include <stdio.h>

#define N 1000000 // Size of vectors

__global__ void vectorAdd(int *a, int *b, int *c) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < N)
        c[tid] = a[tid] + b[tid];
}

int main() {
    int *a, *b, *c; // Host vectors
    int *d_a, *d_b, *d_c; // Device vectors

    // Allocate memory on host
    a = (int*)malloc(N * sizeof(int));
    b = (int*)malloc(N * sizeof(int));
    c = (int*)malloc(N * sizeof(int));

    // Allocate memory on device
    cudaMalloc(&d_a, N * sizeof(int));
    cudaMalloc(&d_b, N * sizeof(int));
    cudaMalloc(&d_c, N * sizeof(int));

    // Initialize vectors on host
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i;
    }

    // Copy vectors from host to device
    cudaMemcpy(d_a, a, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, N * sizeof(int), cudaMemcpyHostToDevice);

    // Define grid and block sizes
    int blockSize = 256;
    int gridSize = (int)ceil((float)N/blockSize);

    // Launch kernel
    vectorAdd<<<gridSize, blockSize>>>(d_a, d_b, d_c);

    // Copy result back to host
    cudaMemcpy(c, d_c, N * sizeof(int), cudaMemcpyDeviceToHost);

    // Verify results
    for (int i = 0; i < 10; i++) {
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
    }

    // Free memory
    free(a); free(b); free(c);
    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);

    return 0;
}

