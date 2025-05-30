#include <iostream>
#include <vector>
#include <mpi.h>
#include <chrono>

using namespace std;

// Function to compute scalar product of two blocks
double computeScalarProduct(const vector<double>& vec1, const vector<double>& vec2, int size) {
    double result = 0.0;
    for (int i = 0; i < size; ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 100000000; // Size of vectors
    const int block_size = n / size;

    if (rank == 0 && n % size != 0) {
        cout << "Error: Vector size is not divisible by number of processes." << endl;
        MPI_Finalize();
        return 1;
    }

    vector<double> vec1(n);
    vector<double> vec2(n);

    if (rank == 0) {
        // Initialize vectors
        for (int i = 0; i < n; ++i) {
            vec1[i] = 1;
            vec2[i] = 1;
        }
    }

    // Scatter the vectors
    double start_time = MPI_Wtime();
    vector<double> local_vec1(block_size);
    vector<double> local_vec2(block_size);
    MPI_Scatter(&vec1[0], block_size, MPI_DOUBLE, &local_vec1[0], block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(&vec2[0], block_size, MPI_DOUBLE, &local_vec2[0], block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double final_result = 0.0;
    if (rank > 0) {
        MPI_Recv(&final_result, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << final_result << " " << rank << endl;
    }
    // Compute local scalar product
    double local_result = computeScalarProduct(local_vec1, local_vec2, block_size);
    final_result += local_result;

    // Send local result to next process
    if (rank < size - 1) {
        MPI_Send(&final_result, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    }

    // Output final result
    if (rank == size - 1) {
        double end_time = MPI_Wtime();
        double duration = end_time - start_time;
        cout << "Final result: " << final_result << endl;
        cout << "Time taken for computation: " << duration << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
