#include <cstdio>
#include <vector>
#include <mpi.h>

void print_ordered(double t);

int main(int argc, char *argv[])
{
    int i, myid, ntasks, nrecv, source, destination;
    constexpr int size = 10000000;
    std::vector<int> message(size);
    std::vector<int> receiveBuffer(size);
    MPI_Status status;
    double t0, t1;

    // Initialize MPI and fill ntasks and myid
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // Initialize buffers
    for (i = 0; i < size; i++) {
        message[i] = myid;
        receiveBuffer[i] = -1;
    }


    // set up source and destination for send and receive
    if (myid == 0) {
        source = MPI_PROC_NULL;
        destination = myid + 1;
    }
    else if (myid == (ntasks - 1)) {
        source = myid - 1;
        destination = MPI_PROC_NULL;
    }
    else {
        source = myid - 1;
        destination = myid + 1;
    }
        
    // Start measuring the time spent in communication
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();

    // Send and receive with MPI_Sendrecv     
    MPI_Sendrecv(message.data(), size, MPI_INT, destination, myid + 1, receiveBuffer.data(), 
                 size, MPI_INT, source, myid, MPI_COMM_WORLD, &status);

    printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
           myid, size, myid + 1, destination);

    // Find how much data was received
    MPI_Get_count(&status, MPI_INT, &nrecv);
    printf("Receiver: %d. Received %d elements, the first of which is %d.\n",
           myid, nrecv, receiveBuffer[0]);

    // Finalize measuring the time and print it out
    t1 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    fflush(stdout);

    print_ordered(t1 - t0);

    MPI_Finalize();
    return 0;
}

void print_ordered(double t)
{
    int i, rank, ntasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (rank == 0) {
        printf("Time elapsed in rank %2d: %6.3f\n", rank, t);
        for (i = 1; i < ntasks; i++) {
            MPI_Recv(&t, 1, MPI_DOUBLE, i, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Time elapsed in rank %2d: %6.3f\n", i, t);
        }
    } else {
        MPI_Send(&t, 1, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
    }
}
