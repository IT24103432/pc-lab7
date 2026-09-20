#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_ELEMENTS 1000000

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int my_rank, comm_sz;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int *data = (int *)malloc(TOTAL_ELEMENTS * sizeof(int));
    if (!data) {
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if (my_rank == 0) {
        for (int i = 0; i < TOTAL_ELEMENTS; i++) {
            data[i] = i + 1;
        }
        printf("[Master] Initialized buffer of %d integers.\n", TOTAL_ELEMENTS);
    }

    double t_start = MPI_Wtime();

    MPI_Bcast(data, TOTAL_ELEMENTS, MPI_INT, 0, MPI_COMM_WORLD);

    int base_chunk = TOTAL_ELEMENTS / comm_sz;
    int first_idx = my_rank * base_chunk;
    int last_idx = (my_rank == comm_sz - 1) ? TOTAL_ELEMENTS : first_idx + base_chunk;

    long long my_partial_sum = 0;
    for (int i = first_idx; i < last_idx; i++) {
        my_partial_sum += data[i];
    }

    printf("Rank %d processed [%d, %d) -> sum = %lld\n",
           my_rank, first_idx, last_idx, my_partial_sum);

    const int MSG_TAG = 42;

    if (my_rank != 0) {
        MPI_Send(&my_partial_sum, 1, MPI_LONG_LONG, 0, MSG_TAG, MPI_COMM_WORLD);
    } else {
        long long grand_total = my_partial_sum;

        for (int count = 1; count < comm_sz; count++) {
            long long incoming_val = 0;
            MPI_Recv(&incoming_val, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, MSG_TAG,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            grand_total += incoming_val;
        }

        double t_end = MPI_Wtime();
        long long analytical_sum = (long long)TOTAL_ELEMENTS * (TOTAL_ELEMENTS + 1) / 2;

        printf("\n--------------------------------------\n");
        printf("Computed Total : %lld\n", grand_total);
        printf("Expected Total : %lld\n", analytical_sum);
        printf("Status         : %s\n", (grand_total == analytical_sum) ? "PASSED" : "FAILED");
        printf("Execution Time : %.6f seconds\n", t_end - t_start);
        printf("--------------------------------------\n");
    }

    free(data);
    MPI_Finalize();
    return 0;
}