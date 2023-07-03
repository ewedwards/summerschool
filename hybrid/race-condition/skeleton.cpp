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

    sum = 0.0;
    /* The problem is that each thread independently reads and
    updates 'sum', so if both read 'sum = 1' but thread 1 performs
    sum += vecA[3] and thread 2 performs sum += vecA[4], then while
    sum should update by both operations, once the slower thread
    finishes it will overwrite the answer from the quicker thread
     */
    #pragma omp parallel for default(shared) private(i)
    for (i = 0; i < NX; i++) {
        sum += vecA[i];
    }
    printf("Sum: %ld\n", sum);

    return 0;
}
