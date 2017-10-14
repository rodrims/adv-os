#include <stdio.h>
#include "mpi.h"

int num_processes = 0;

void dissemination_barrier();
int ceil_log_2(int x);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    // do work
    dissemination_barrier();

    MPI_Finalize();
    return 0;
}

void dissemination_barrier()
{
    int my_id, my_dst, my_src;
    int num_rounds = ceil_log_2(num_processes);
    int tag = 1;
    int my_msg = 1; // true
	MPI_Status mpi_result;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    for (int curr_round = 0; curr_round < num_rounds; curr_round++)
    {
        int tmp_src;

        my_dst = (my_id + (1 << curr_round)) % num_processes;
        tmp_src = (my_id - (1 << curr_round));
        my_src = tmp_src < 0 ? num_processes - tmp_src : tmp_src;

        MPI_Send(&my_msg, 1, MPI_INT, my_dst, tag, MPI_COMM_WORLD);
        MPI_Recv(&my_msg, 1, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);

        printf("Round %d, proc %d: received message from proc %d", curr_round, my_id, my_msg);
    }
}

int ceil_log_2(int x)
{
    int exp = 0;

    while (1 << exp < x)
    {
        exp++;
    }

    return exp;
}
