#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Function to perform Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Function to perform Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Function to generate random array
void generateRandomArray(int arr[], int n) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;
    }
}

// Function to print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    const int n = 10000;
    int arr[n], arr_copy[n];

    generateRandomArray(arr, n);
    copy(begin(arr), end(arr), begin(arr_copy));

    double start_time, end_time;

    // Sequential Bubble Sort
    start_time = omp_get_wtime();
    bubbleSort(arr, n);
    end_time = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end_time - start_time << " seconds" << endl;

    // Parallel Bubble Sort
    copy(begin(arr_copy), end(arr_copy), begin(arr));
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        bubbleSort(arr, n);
    }
    end_time = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end_time - start_time << " seconds" << endl;

    // Sequential Merge Sort
    start_time = omp_get_wtime();
    mergeSort(arr_copy, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end_time - start_time << " seconds" << endl;

    // Parallel Merge Sort
    copy(begin(arr), end(arr), begin(arr_copy));
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            mergeSort(arr_copy, 0, n - 1);
        }
    }
    end_time = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end_time - start_time << " seconds" << endl;

    return 0;
}

