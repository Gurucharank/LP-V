#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

using namespace std;

// Parallel Bubble Sort using Odd-Even Transposition
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n; j += 2) {
            if (arr[j - 1] > arr[j]) {
                swap(arr[j - 1], arr[j]);
            }
        }
        // Even phase
        #pragma omp parallel for
        for (int j = 2; j < n; j += 2) {
            if (arr[j - 1] > arr[j]) {
                swap(arr[j - 1], arr[j]);
            }
        }
    }
}

// Standard Merge Function
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// Parallel Merge Sort using Tasks
void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Use tasks only if the array is large enough to justify overhead
        if (r - l > 1000) {
            #pragma omp task
            parallelMergeSort(arr, l, m);
            
            #pragma omp task
            parallelMergeSort(arr, m + 1, r);
            
            #pragma omp taskwait
        } else {
            parallelMergeSort(arr, l, m);
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main() {
    const int n = 10000;
    int *arr = new int[n];
    int *copy_arr = new int[n];

    // Initialize
    for(int i=0; i<n; i++) arr[i] = rand() % 1000;
    
    double start, end;

    // --- BUBBLE SORT ---
    for(int i=0; i<n; i++) copy_arr[i] = arr[i];
    start = omp_get_wtime();
    parallelBubbleSort(copy_arr, n);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort: " << end - start << " seconds" << endl;

    // --- MERGE SORT ---
    for(int i=0; i<n; i++) copy_arr[i] = arr[i];
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(copy_arr, 0, n - 1);
    }
    end = omp_get_wtime();
    cout << "Parallel Merge Sort:  " << end - start << " seconds" << endl;

    delete[] arr;
    delete[] copy_arr;
    return 0;
}