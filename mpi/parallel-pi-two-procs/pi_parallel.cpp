#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 840;

int main(int argc, char** argv)
{
  int myid, ntasks, istart, istop;
  double pi = 0.0;
  double receive;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  printf("Computing approximation to pi with N=%d\n", n);

  if (myid == 0) {
    istart = 1; istop = n / 2;
   
    for (int i=istart; i <= istop; i++) {
      double x = (i - 0.5) / n;
      pi += 1.0 / (1.0 + x*x);
    }
    MPI_Recv(&receive, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &status);
    pi += receive;
    pi *= 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  }

  else {
    istart = n / 2 + 1; istop = n;
    for (int i=istart; i <= istop; i++) {
      double x = (i - 0.5) / n;
      pi += 1.0 / (1.0 + x*x);
    }
    MPI_Send(&pi, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }

}
