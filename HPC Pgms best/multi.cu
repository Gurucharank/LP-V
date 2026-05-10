#include <stdio.h>

#define N 512 // Matrix size

__global__ void matrixMul(int *a, int *b, int *c) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        int sum = 0;
        for (int i = 0; i < N; ++i) {
            sum += a[row * N + i] * b[i * N + col];
        }
        c[row * N + col] = sum;
    }
}

int main() {
    int *a, *b, *c; // Host matrices
    int *d_a, *d_b, *d_c; // Device matrices

    // Allocate memory on host
    a = (int*)malloc(N * N * sizeof(int));
    b = (int*)malloc(N * N * sizeof(int));
    c = (int*)malloc(N * N * sizeof(int));

    // Allocate memory on device
    cudaMalloc(&d_a, N * N * sizeof(int));
    cudaMalloc(&d_b, N * N * sizeof(int));
    cudaMalloc(&d_c, N * N * sizeof(int));

    // Initialize matrices on host
    for (int i = 0; i < N * N; ++i) {
        a[i] = i;
        b[i] = i;
    }

    // Copy matrices from host to device
    cudaMemcpy(d_a, a, N * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, N * N * sizeof(int), cudaMemcpyHostToDevice);

    // Define grid and block sizes
    dim3 blockSize(16, 16);
    dim3 gridSize((N + blockSize.x - 1) / blockSize.x, (N + blockSize.y - 1) / blockSize.y);

    // Launch kernel
    matrixMul<<<gridSize, blockSize>>>(d_a, d_b, d_c);

    // Copy result back to host
    cudaMemcpy(c, d_c, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    // Verify results
    printf("First element of result matrix: %d\n", c[0]);

    // Free memory
    free(a); free(b); free(c);
    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);

    return 0;
}

