Exercise 06 - Prefix Sums with Scan
Source: sum_scan.c

MPI_Allreduce was replaced with MPI_Scan. Each rank gets the prefix sum of
ranks 0..rank; sum_before_me = prefix_sum - local_sum. The last rank's
prefix_sum is the total.

Output (mpirun -np 4 ./sum_scan):

Root filled array with values 1 to 1000000
  Rank 0: local_sum = 31250125000, prefix_sum = 31250125000, sum_before_me = 0, prefix check (K=250000) = OK
  Rank 1: local_sum = 93750125000, prefix_sum = 125000250000, sum_before_me = 31250125000, prefix check (K=500000) = OK
  Rank 2: local_sum = 156250125000, prefix_sum = 281250375000, sum_before_me = 125000250000, prefix check (K=750000) = OK
  Rank 3: local_sum = 218750125000, prefix_sum = 500000500000, sum_before_me = 281250375000, prefix check (K=1000000) = OK

[Scan] Last rank (3) prefix_sum = 500000500000
[Scan] Expected total        = 500000500000
[Scan] Correct?              = YES
[Scan] Time                  = 0.0014 sec

