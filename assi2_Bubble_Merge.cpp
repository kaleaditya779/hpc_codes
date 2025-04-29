#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Sequential Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; ++i)
        for (int j = 0; j < n-i-1; ++j)
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
}

// Parallel Bubble Sort using OpenMP
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        #pragma omp parallel for
        for (int j = i % 2; j < n-1; j += 2) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

// Merge function
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid+1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int l = 0; l < temp.size(); ++l)
        arr[left + l] = temp[l];
}

// Sequential Merge Sort
void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid+1, right);
    merge(arr, left, mid, right);
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    if (left >= right) return;
    int mid = (left + right) / 2;

    if (depth <= 3) { // Limit depth of parallelism to avoid oversubscription
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid, depth+1);
            #pragma omp section
            parallelMergeSort(arr, mid+1, right, depth+1);
        }
    } else {
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);
    }
    merge(arr, left, mid, right);
}

int main() {
    int n = 10000;
    vector<int> arr1(n), arr2(n), arr3(n), arr4(n);

    srand(time(0));
    for (int i = 0; i < n; ++i) {
        int num = rand();
        arr1[i] = arr2[i] = arr3[i] = arr4[i] = num;
    }

    // Sequential Bubble Sort
    double start = omp_get_wtime();
    bubbleSort(arr1);
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end - start << " seconds\n";

    // Parallel Bubble Sort
    start = omp_get_wtime();
    parallelBubbleSort(arr2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end - start << " seconds\n";

    // Sequential Merge Sort
    start = omp_get_wtime();
    mergeSort(arr3, 0, n-1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << " seconds\n";

    // Parallel Merge Sort
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelMergeSort(arr4, 0, n-1);
        }
    }
    
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << " seconds\n";

    return 0;
}
