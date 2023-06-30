#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
    // define some stuff
    int size, rank, len;
    char name[MPI_MAX_PROCESSOR_NAME];

    // main body
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);

    // print some stuff
    printf("Hello from rank %d!\n", rank);
    printf("I'm on %s.\n", name);
    if (rank==0) {
        printf("There are %d MPI processes running.\n", size);
    }
    
    //double buffer[3] = {0.0};

    //if (rank == 0) {
    //    MPI_Send(buffer, 3, MPI_DOUBLE, 1, 123, MPI_COMM_WORLD);
    //}
    //else {
    //    MPI_Status status;
    //    MPI_Receive(buffer, 3, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
    //}

    MPI_Finalize();
}
