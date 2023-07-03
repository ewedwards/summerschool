#include <cstdio>

#define NX 102400

int main(void)
{
    long vecA[NX];
    long sum, psum, sumex;
    int i;

    /* Initialization of the vectors */
    for (i = 0; i < NX; i++) {
        vecA[i] = (long) i + 1;
    }

    sumex = (long) NX * (NX + 1) / ((long) 2);
    printf("Arithmetic sum formula (exact):                  %ld\n",
           sumex);

    // using private partial sums and critical section
    sum = 0.0;
    #pragma omp parallel default(shared) private(i, psum)
    {
        psum = 0.0;
        #pragma omp for
        for (i = 0; i < NX; i++) {
            psum += vecA[i];
        }
        #pragma omp critical(par_sum) //TODO: Where does 'par_sum' come from?
        sum += psum;
    }
    printf("Sum using partial sums + critical section: %ld\n", sum);

    // using reduction
    sum = 0.0;
    #pragma omp parallel for default(shared) private(i) reduction(+:sum)
    for (i = 0; i < NX; i++) {
        sum += vecA[i];
    }
    printf("Sum: %ld\n", sum);

    return 0;
}
