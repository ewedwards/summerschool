#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <hiprand/hiprand.h>
#include <time.h>


float cpu_pi(int n)
{
    int inside, i;
    float *x, *y;

    x = (float *)malloc(n * sizeof(float));
    y = (float *)malloc(n * sizeof(float));

    for (i = 0; i < n; i++) {
        x[i] = (float)rand() / (float)RAND_MAX;
        y[i] = (float)rand() / (float)RAND_MAX;
    }

    inside = 0;
    for (i = 0; i < n; i++) {
        if (x[i]*x[i] + y[i]*y[i] < 1.0) {
            inside++;
        }
    }

    free(x);
    free(y);

    return 4.0 * (float)inside / (float)n;
}


float gpu_pi(size_t n)
{
    hiprandGenerator_t g;
    int istat;
    int inside;
    float *x, *y, pi;

    pi = 0;

    x = (float *)malloc(n * sizeof(float));
    y = (float *)malloc(n * sizeof(float));
    
    // TODO start: allocate x and y in the device with OpenMP enter data
    #pragma omp target enter data map(alloc:x[0:n],y[0:n])
    // TODO end

    inside = 0;

    istat = hiprandCreateGenerator(&g, HIPRAND_RNG_PSEUDO_DEFAULT);

    // TODO start: use device pointer for HIP random generator calls
    #pragma omp target data use_device_ptr(x,y)
    {
        istat = hiprandGenerateUniform(g, x, n);
        if (istat != HIPRAND_STATUS_SUCCESS) printf("Error in hiprandGenerate: %d\n", istat);
        istat = hiprandGenerateUniform(g, y, n);
        if (istat != HIPRAND_STATUS_SUCCESS) printf("Error in hiprandGenerate: %d\n", istat);
    }
    // TODO end

    // TODO start: execute the loop in parallel in device
    #pragma omp target teams distribute parallel for reduction(+:inside)
        for (int i = 0; i < n; i++) {
            if (x[i]*x[i] + y[i]*y[i] < 1.0) {
                inside++;
            }
        }
    // TODO end

    // TODO start: deallocate x and y in the device with OpenMP exit data
    #pragma omp target exit data map(delete:x[0:n],y[0:n])
    // TODO end

    free(x);
    free(y);

    istat = hiprandDestroyGenerator(g);
    if (istat != HIPRAND_STATUS_SUCCESS) {
        fprintf(stderr, "Error in hiprandDestroyGenerator: %d\n", istat);
    }

    pi = 4.0 * (float)inside / (float)n;

    return pi;
}


int main(int argc, char *argv[])
{
    int nsamples;
    float pi_cpu, pi_gpu;
    double cpu_time_used;
    clock_t start, end;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s N\n  where N is the number samples\n", argv[0]);
        exit(EXIT_FAILURE);
    } else {
        nsamples = atoi(argv[1]);
    }
    
    start = clock();
    pi_cpu = cpu_pi(nsamples);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Pi = %9.6f\n", pi_cpu);
    printf("Time elapsed: %9.6f s\n", cpu_time_used);

    start = clock();
    pi_gpu = gpu_pi(nsamples);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Pi = %9.6f\n", pi_gpu);
    printf("Time elapsed: %9.6f s\n", cpu_time_used);

    return 0;
}
