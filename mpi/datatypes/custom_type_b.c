#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank;
    int array[8][8];

    // Declare a variable storing the MPI datatype
    MPI_Datatype indexedtype;
    int counts[4];
    int displs[4];


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize arrays
    if (rank == 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                array[i][j] = (i + 1) * 10 + j + 1;
            }
        }
    } else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                array[i][j] = 0;
            }
        }
    }

    // Print data on rank 0
    if (rank == 0) {
        printf("Data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    // set values for counts and displs
    for (int i=0; i<4; i++) {
        counts[i] = i + 1;
        displs[i] = 17*i;
    }

    // Create datatype
    MPI_Type_indexed(4, counts, displs, MPI_INT , &indexedtype);
    MPI_Type_commit(&indexedtype); // must be called to use indexedtype

    // Send data from rank 0 to rank 1
    if (rank==0) {
        MPI_Send(array, 1, indexedtype, 1, 1, MPI_COMM_WORLD); // start at second element of array
    }
    else if (rank==1) {
        MPI_Recv(array, 1, indexedtype, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Free datatype
    MPI_Type_free(&indexedtype);

    // Print received data
    if (rank == 1) {
        printf("Received data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}
