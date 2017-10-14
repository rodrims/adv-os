#include <stdio.h>
#include "mpi.h"

int num_processes = 0;

void mcs_tree_barrier();

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    // do work
    mcs_tree_barrier();

    MPI_Finalize();
    return 0;
}

void mcs_tree_barrier()
{
    int my_id, my_dst, my_src;
    int tag = 1;
    int my_msg = 1; // true
    MPI_Status mpi_result;

    int has_child[4];
    int child_not_ready[4];
    int has_wakeup_child[2];

    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    // initialize arrival tree
    for (int i = 0; i < 4; i++)
    {
        if ((my_id * 4) + (i + 1) < num_processes)
        {
            has_child[i] = 1;
        } else
        {
            has_child[i] = 0;
        }
        
        child_not_ready[i] = has_child[i];
    }

    // wait for children arrival
    while (child_not_ready[0] || child_not_ready[1] || child_not_ready[2] || child_not_ready[3])
    {
        if (child_not_ready[0])
        {
            my_src = (my_id * 4) + 1;
            MPI_Recv(&my_msg, 1, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
            child_not_ready[0] = 0;
        }
        else if (child_not_ready[1])
        {
            my_src = (my_id * 4) + 2;
            MPI_Recv(&my_msg, 1, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
            child_not_ready[1] = 0;
        }
        else if (child_not_ready[2])
        {
            my_src = (my_id * 4) + 3;
            MPI_Recv(&my_msg, 1, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
            child_not_ready[2] = 0;
        }
        else if (child_not_ready[3])
        {
            my_src = (my_id * 4) + 4;
            MPI_Recv(&my_msg, 1, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
            child_not_ready[3] = 0;
        }
    }

    // if not root of tree, signal parent
    if (my_id != 0)
    {
        my_dst = my_id % 4 == 0 ? (my_id - 1 ) / 4 : my_id / 4; // integer (floor) division
        MPI_Send(&my_msg, 1, MPI_INT, my_dst, tag, MPI_COMM_WORLD);
    }

    // initialize wakeup tree
    for (int i = 0; i < 2; i++)
    {
        if ((my_id * 2) + (i + 1) < num_processes)
        {
            has_wakeup_child[i] = 1;
        }
        else
        {
            has_wakeup_child[i] = 0;
        }
    }

    // if not root of tree, wait for parent wakeup
    if (my_id != 0)
    {
        my_src = my_id % 2 == 0 ? (my_id - 1) / 2 : my_id / 2; // integer (floor) division
        MPI_Recv(&my_msg, 1, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
    }

    for (int i = 0; i < 2; i++)
    {
        if (has_wakeup_child[i])
        {
            my_dst = (my_id * 2) + (i + 1);
            MPI_Send(&my_msg, 1, MPI_INT, my_dst, tag, MPI_COMM_WORLD);
        }
    }
}
