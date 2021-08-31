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
	printf("\nDentro del parallel\nthread %d imprime dyn-var %d\n",omp_get_thread_num(),dyn_var);
	printf("thread %d imprime nthreads_var %d\n",omp_get_thread_num(),nthreads_var);
	printf("thread %d imprime thread-limit-var %d\n",omp_get_thread_num(),thread_limit_var);
	printf("thread %d imprime run-sched-var:\n",omp_get_thread_num());
	if(omp_sched_dynamic == kind) printf("Tipo dynamic, modificador %d\n\n",modifier);
	else if(omp_sched_guided == kind) printf("Tipo guided, modificador %d\n\n",modifier);
	else printf("Tipo static, modificador %d\n\n",modifier);
	}
    }    

    printf("Fuera de 'parallel for' suma=%d\n",suma);
    printf("\ndyn-var %d\n",dyn_var);
    printf("nthreads_var %d\n",nthreads_var);
    printf("thread-limit-var %d\n",thread_limit_var); 
    printf("run-sched-var:\n");
	if(omp_sched_dynamic == kind) printf("Tipo dynamic, modificador %d\n\n",modifier);
	else if(omp_sched_guided == kind) printf("Tipo guided, modificador %d\n\n",modifier);
	else printf("Tipo static, modificador %d\n\n",modifier);
}

