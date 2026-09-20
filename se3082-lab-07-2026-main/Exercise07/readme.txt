Exercise 07 - Summary Comparison

1. Comparison

Program         | Collectives               | Memory                  | Manual sum on root | Result available
----------------|---------------------------|-------------------------|--------------------|---------------------------
sum_bcast       | MPI_Bcast + Send/Recv     | Full array on all       | Yes                | Root only
sum_scatter     | MPI_Scatter + Send/Recv   | Full on root, chunk     | Yes                | Root only
sum_gather      | MPI_Scatter, MPI_Gather   | Full on root, chunk     | Yes                | Root only
sum_reduce      | MPI_Scatter, MPI_Reduce   | Full on root, chunk     | No                 | Root only
sum_allreduce   | MPI_Scatter, MPI_Allreduce| Full on root, chunk     | No                 | All processes
sum_scan        | MPI_Scatter, MPI_Scan     | Full on root, chunk     | No                 | Different per rank (last rank = total)


2. Execution times (seconds, median of 5 runs)

Program         | np=2   | np=4   | np=8
----------------|--------|--------|-------
sum_bcast       | 0.0028 | 0.0063 | 0.0137
sum_scatter     | 0.0030 | 0.0033 | 0.0038
sum_gather      | 0.0029 | 0.0033 | 0.0039
sum_reduce      | 0.0033 | 0.0038 | 0.0043
sum_allreduce   | 0.0027 | 0.0030 | 0.0037
sum_scan        | 0.0036 | 0.0043 | 0.0066

Fastest: sum_allreduce (Scatter-based versions are all close).
Slowest: sum_bcast, and it gets worse as processes increase.

Why: Bcast sends the full 1,000,000-element array to every process, so the
data sent grows with the number of processes. Scatter sends each process
only N/P elements, so total data stays about N. The collection step moves
only one value per process, so Gather, Reduce and Allreduce differ very
little at this size.


3. When to use MPI_Scan instead of MPI_Allreduce

Allreduce gives every process the same global value. Scan gives each
process the total of everything up to its rank.

Example: assigning unique global IDs to records spread across processes.
If rank 0 has 120 records, rank 1 has 95 and rank 2 has 143, MPI_Scan on
the counts gives 120, 215, 358. Each rank's starting ID is
prefix - local_count (0, 120, 215). Allreduce would only give the total
(358), which is not enough to know where each rank's IDs start.

