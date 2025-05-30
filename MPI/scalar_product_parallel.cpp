#include <iostream>
#include <vector>
#include <mpi.h>

#define VECTOR_SIZE 100000000
#define TAG 0

using namespace std;

// Function to compute scalar product of two vectors
double scalarProduct(const vector<double>& v1, const vector<double>& v2, int n) {
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Divide vectors into parts
    int n = VECTOR_SIZE / size;
    vector<double> localVector1(n);
    vector<double> localVector2(n);

    // Initialize vectors
    for (int i = 0; i < n; ++i) {
        localVector1[i] = rank * n + i;
        localVector2[i] = (rank * n + i) * 2;
    }

    // Time measurement
    double startTime, endTime;
    startTime = MPI_Wtime();

    // Compute local scalar product
    double localResult = scalarProduct(localVector1, localVector2, n);

    // Reduce results to process 0
    double globalResult;
    MPI_Reduce(&localResult, &globalResult, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    endTime = MPI_Wtime();

    if (rank == 0) {
        cout << "Scalar Product: " << globalResult << endl;
        cout << "Time taken: " << endTime - startTime << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
