#include <iostream>
#include <vector>
#include <numeric>
#include <omp.h>
#include <random>

// Функция для параллельного вычисления суммы элементов вектора
void parallel_accumulate(int start, int shift_size, int size, std::vector<double> a, double& total) {
    double res = 0;
    for (int i = start; i < size; i += shift_size) {
        res += a[i];
    }
    total += res;
}

int main(int argc, char **argv) {
    const int size = 100000000;
    int num_threads = std::stoi(argv[1]);
    std::vector<double> data(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (int i = 0; i < size; ++i) {
        data[i] = 1;//dis(gen);
    }

    double start, stop;
    start = omp_get_wtime();

    // Устанавливаем количество потоков OpenMP
    omp_set_num_threads(num_threads);

    // Разделение работы между потоками OpenMP
    double  total = 0;
    #pragma omp parallel shared(total)
    {
        int thread_id = omp_get_thread_num();
        int shift_size = num_threads;
        int start_index = thread_id;

        parallel_accumulate(start_index, shift_size, size, data, total);
    }
    
    stop = omp_get_wtime();
    
    std::cout << "Total sum: " << total << std::endl;
    std::cout << "\nTime taken: " << stop - start << " seconds" << std::endl;

    return 0;
}
