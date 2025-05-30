#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

void merge(std::vector<double>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<double> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            int i = 0, j = 0, k = l;
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
        }
        #pragma omp section
        {
            int i = n1 - 1, j = n2 - 1, k = r;
            while (i >= 0 && j >= 0) {
                if (L[i] >= R[j]) {
                    arr[k] = L[i];
                    i--;
                } else {
                    arr[k] = R[j];
                    j--;
                }
                k--;
            }

            while (j >= 0) {
                arr[k] = R[j];
                j--;
                k--;
            }
        }
    }
}

void mergeSort(std::vector<double>& arr, int l, int r) {
    if (l >= r)
        return;

    int m = l + (r - l) / 2;

    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    merge(arr, l, m, r);
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

    double start_time = omp_get_wtime();

    mergeSort(arr, 0, n - 1);

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

//    for (int i = 0; i < n; i++)
//        std::cout << arr[i] << " ";

    std::cout << "\nElapsed time: " << elapsed_time << " seconds" << std::endl;

    return 0;
}
