#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_ELEMENTS 1000000

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int my_rank, comm_sz;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int block_size = TOTAL_ELEMENTS / comm_sz;

    int *full_dataset = NULL;
    if (my_rank == 0) {
        full_dataset = (int *)malloc(TOTAL_ELEMENTS * sizeof(int));
        if (!full_dataset) {
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        for (int i = 0; i < TOTAL_ELEMENTS; i++) {
            full_dataset[i] = i + 1;
        }
        printf("[Master] Seeded dataset with %d values.\n", TOTAL_ELEMENTS);
    }

    int *sub_vector = (int *)malloc(block_size * sizeof(int));
    if (!sub_vector) {
        if (my_rank == 0) free(full_dataset);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    double t_start = MPI_Wtime();

    MPI_Scatter(full_dataset, block_size, MPI_INT,
                sub_vector, block_size, MPI_INT,
                0, MPI_COMM_WORLD);

    long long my_partial_sum = 0;
    for (int i = 0; i < block_size; i++) {
        my_partial_sum += sub_vector[i];
    }

    long long run_sum = 0;
    MPI_Scan(&my_partial_sum, &run_sum, 1, MPI_LONG_LONG,
             MPI_SUM, MPI_COMM_WORLD);

    double t_end = MPI_Wtime();

    long long prior_offset = run_sum - my_partial_sum;
    long long count_up_to = (long long)(my_rank + 1) * block_size;
    long long analytical_prefix = count_up_to * (count_up_to + 1) / 2;

    printf("Rank %d: local = %lld, scan = %lld, offset = %lld, match = %s\n",
           my_rank, my_partial_sum, run_sum, prior_offset,
           (run_sum == analytical_prefix) ? "YES" : "NO");

    if (my_rank == comm_sz - 1) {
        long long grand_expected = (long long)TOTAL_ELEMENTS * (TOTAL_ELEMENTS + 1) / 2;

        printf("\n--------------------------------------\n");
        printf("Tail Rank (%d) Scan : %lld\n", my_rank, run_sum);
        printf("Expected Total      : %lld\n", grand_expected);
        printf("Status              : %s\n", (run_sum == grand_expected) ? "PASSED" : "FAILED");
        printf("Execution Time      : %.6f seconds\n", t_end - t_start);
        printf("--------------------------------------\n");
    }

    if (my_rank == 0) {
        free(full_dataset);
    }
    free(sub_vector);

    MPI_Finalize();
    return 0;
}