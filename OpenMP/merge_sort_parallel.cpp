#include <iostream>
#include <vector>
#include <random>
#include "omp.h"
#include <sys/time.h>

void merge(std::vector<double>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<double> L(n1);
    std::vector<double> R(n2);

     #pragma omp parallel for
    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }

     #pragma omp parallel for
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

void mergeSort(std::vector<double>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        #pragma omp task
        mergeSort(arr, l, m);

        #pragma omp task
        mergeSort(arr, m + 1, r);

        #pragma omp taskwait
        merge(arr, l, m, r);
    }
}

int main(int argc, char **argv) {
    std::vector<double> arr;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-100.0, 100.0); // Range of random double values
    int n = std::stoi(argv[1]);
    for (int i = 0; i < n; i++) {
        double random_double = dis(gen);
        arr.push_back(random_double);
    }
    
    double start, stop;

    start = omp_get_wtime();
    
    #pragma omp parallel
    {
        #pragma omp single
        mergeSort(arr, 0, n - 1);
    }
    
    stop = omp_get_wtime();
    
    std::cout << "\nTime taken: " << stop - start << " seconds" << std::endl;

    return 0;
}
