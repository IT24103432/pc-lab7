Exercise 08 - Compile and Run

Makefile is in the repository root.
  make        compiles all 6 programs
  make run    runs all 6 with 4 processes

Output (make run):

mpicc -O2 -Wall -o Exercise01/sum_bcast Exercise01/sum_bcast.c
mpicc -O2 -Wall -o Exercise02/sum_scatter Exercise02/sum_scatter.c
mpicc -O2 -Wall -o Exercise03/sum_gather Exercise03/sum_gather.c
mpicc -O2 -Wall -o Exercise04/sum_reduce Exercise04/sum_reduce.c
mpicc -O2 -Wall -o Exercise05/sum_allreduce Exercise05/sum_allreduce.c
mpicc -O2 -Wall -o Exercise06/sum_scan Exercise06/sum_scan.c

===== Exercise01/sum_bcast (np=4) =====
Root filled array with values 1 to 1000000
  Rank 0: summed indices [0, 250000) => local_sum = 31250125000
  Rank 1: summed indices [250000, 500000) => local_sum = 93750125000
  Rank 2: summed indices [500000, 750000) => local_sum = 156250125000
  Rank 3: summed indices [750000, 1000000) => local_sum = 218750125000

[Bcast] Total sum   = 500000500000
[Bcast] Expected    = 500000500000
[Bcast] Correct?    = YES
[Bcast] Time        = 0.0070 sec

===== Exercise02/sum_scatter (np=4) =====
Root filled array with values 1 to 1000000
  Rank 0: received 250000 elements [1 .. 250000] => local_sum = 31250125000
  Rank 1: received 250000 elements [250001 .. 500000] => local_sum = 93750125000
  Rank 2: received 250000 elements [500001 .. 750000] => local_sum = 156250125000
  Rank 3: received 250000 elements [750001 .. 1000000] => local_sum = 218750125000

[Scatter] Total sum   = 500000500000
[Scatter] Expected    = 500000500000
[Scatter] Correct?    = YES
[Scatter] Time        = 0.0044 sec

===== Exercise03/sum_gather (np=4) =====
Root filled array with values 1 to 1000000
  Rank 0: local_sum = 31250125000
  Rank 1: local_sum = 93750125000
  Rank 2: local_sum = 156250125000
  Rank 3: local_sum = 218750125000

[Gather] Total sum   = 500000500000
[Gather] Expected    = 500000500000
[Gather] Correct?    = YES
[Gather] Time        = 0.0031 sec

===== Exercise04/sum_reduce (np=4) =====
Root filled array with values 1 to 1000000
  Rank 0: local_sum = 31250125000
  Rank 1: local_sum = 93750125000
  Rank 2: local_sum = 156250125000
  Rank 3: local_sum = 218750125000

[Reduce] Total sum   = 500000500000
[Reduce] Expected    = 500000500000
[Reduce] Correct?    = YES
[Reduce] Time        = 0.0035 sec

===== Exercise05/sum_allreduce (np=4) =====
Root filled array with values 1 to 1000000
  Rank 0: local_sum = 31250125000, total_sum = 500000500000, contribution = 6.25%
  Rank 1: local_sum = 93750125000, total_sum = 500000500000, contribution = 18.75%
  Rank 2: local_sum = 156250125000, total_sum = 500000500000, contribution = 31.25%
  Rank 3: local_sum = 218750125000, total_sum = 500000500000, contribution = 43.75%

[Allreduce] Total sum   = 500000500000
[Allreduce] Expected    = 500000500000
[Allreduce] Correct?    = YES
[Allreduce] Time        = 0.0030 sec

===== Exercise06/sum_scan (np=4) =====
Root filled array with values 1 to 1000000
  Rank 0: local_sum = 31250125000, prefix_sum = 31250125000, sum_before_me = 0, prefix check (K=250000) = OK
  Rank 1: local_sum = 93750125000, prefix_sum = 125000250000, sum_before_me = 31250125000, prefix check (K=500000) = OK
  Rank 2: local_sum = 156250125000, prefix_sum = 281250375000, sum_before_me = 125000250000, prefix check (K=750000) = OK
  Rank 3: local_sum = 218750125000, prefix_sum = 500000500000, sum_before_me = 281250375000, prefix check (K=1000000) = OK

[Scan] Last rank (3) prefix_sum = 500000500000
[Scan] Expected total        = 500000500000
[Scan] Correct?              = YES
[Scan] Time                  = 0.0014 sec

