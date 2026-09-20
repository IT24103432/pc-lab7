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

    long long grand_total = 0;
    MPI_Allreduce(&my_partial_sum, &grand_total, 1, MPI_LONG_LONG,
                  MPI_SUM, MPI_COMM_WORLD);

    double t_end = MPI_Wtime();

    double share_pct = (double)my_partial_sum / (double)grand_total * 100.0;
    printf("Rank %d: partial = %lld, total = %lld, share = %.2f%%\n",
           my_rank, my_partial_sum, grand_total, share_pct);

    if (my_rank == 0) {
        long long analytical_sum = (long long)TOTAL_ELEMENTS * (TOTAL_ELEMENTS + 1) / 2;

        printf("\n--------------------------------------\n");
        printf("Computed Total : %lld\n", grand_total);
        printf("Expected Total : %lld\n", analytical_sum);
        printf("Status         : %s\n", (grand_total == analytical_sum) ? "PASSED" : "FAILED");
        printf("Execution Time : %.6f seconds\n", t_end - t_start);
        printf("--------------------------------------\n");

        free(full_dataset);
    }

    free(sub_vector);
    MPI_Finalize();
    return 0;
}