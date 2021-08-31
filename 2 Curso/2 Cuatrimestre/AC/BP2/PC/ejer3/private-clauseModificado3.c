#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#endif

int main()
{
	int i,n=7;
	int a[n],suma;

	for(i=0;i<n;i++)
	   a[i] = i;
	
	#pragma omp parallel 
	{
	suma = 0;
	#pragma omp for
	for(i=0;i<n;i++)
	{
	   suma += a[i];
	   printf("Hebra %d suma a[%d]/",omp_get_thread_num(),i);
	}
	printf("\n* Hebra %d suma= %d", omp_get_thread_num(), suma);
	}
 printf("\n");
}	
	
