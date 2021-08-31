#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP
    #include <omp.h>
#else
    #define omp_get_thread_num() 0
#endif

int main(int argc, char **argv)
{
    int i, n=200, chunk, a[n], suma=0;
    
    if(argc < 3)
    {
        fprintf(stderr,"\nFalta iteraciones o chunk\n");
        exit(-1);
    }

    n = atoi(argv[1]);
    if (n>200)
        n=200;

    chunk = atoi(argv[2]); 

    for (i=0; i<n; i++)
        a[i] = i; 

	
    int dyn_var = omp_get_dynamic();
    int nthreads_var = omp_get_max_threads();
    int thread_limit_var = omp_get_thread_limit();
    int modifier;
    omp_sched_t kind;
    omp_get_schedule(&kind,&modifier);	
	
 	
    #pragma omp parallel 
    {
    #pragma omp for firstprivate(suma) lastprivate(suma) schedule(dynamic,chunk)
    for (i=0; i<n; i++)
    {
        suma = suma + a[i];
        printf(" thread %d suma a[%d]=%d suma=%d \n",
               omp_get_thread_num(),i,a[i],suma);
    }
	#pragma omp single
	{
	printf("\nDentro del parallel\n");
	printf("\nomp_get_num_threads = %d\n", omp_get_num_threads());
	printf("\nomp_get_num_procs = %d\n", omp_get_num_procs());
	printf("\nomp_in_parallel = %d\n\n", omp_in_parallel());
	}
   }    

    printf("Fuera de 'parallel for' suma=%d\n",suma);
    printf("\nomp_get_num_threads = %d\n", omp_get_num_threads());
    printf("\nomp_get_num_procs = %d\n", omp_get_num_procs());
    printf("\nomp_in_parallel = %d\n\n", omp_in_parallel());
	
}

