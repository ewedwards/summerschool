#include <cstdio>
#include <omp.h>
int main()
{
    int thread_id, nthreads;
    printf("Hello world!\n");
    #pragma omp parallel private(thread_id) shared(nthreads)
    {
        thread_id = omp_get_thread_num();
        #pragma omp single
            nthreads = omp_get_num_threads();
        #pragma omp critical
            printf("... from thread ID %i.\n", thread_id);
    }
    printf("There were %i threads in total.\n", nthreads);
    return 0;
}
