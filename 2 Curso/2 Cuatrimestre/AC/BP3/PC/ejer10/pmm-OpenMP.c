#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <omp.h>



int main(int argc, char const *argv[]){			
	if(argc != 2){					//Esto es para aceptar el argumento N(numero de filas/columnas)
        printf("Error de argumentos %s", argv[0]);
        return(EXIT_FAILURE);
    }
    
    struct timespec cgt1, cgt2;			//Definimos estructuras para medir el tiempo
    double ncgt;

    unsigned int N = atoi(argv[1]);


        double **A,**B,**C;			//Inicializacion dinámica de las matrices 
        A = (double**) malloc(N * sizeof(double*));
	B = (double**) malloc(N * sizeof(double*));
	C = (double**) malloc(N * sizeof(double*));
        for(int i = 0; i < N; ++i){
            A[i] = (double*) malloc(N * sizeof(double));
       	    B[i] = (double*) malloc(N * sizeof(double));
 	    C[i] = (double*) malloc(N * sizeof(double));
	}    

	int i,j,k;
	double ti,tf,tt;

    #pragma omp parallel for schedule(runtime) private(j)
    for(int i = 0; i < N; ++i)
    {					//Se rellenan el vector y la matriz con valores
        for(int j = 0; j < N; ++j)
	{
        		B[i][j] = 3.12345 + i;
			C[i][j] = 2.98765 + j;
			A[i][j] = 0;
	}
    }

	ti = omp_get_wtime();		//Cojemos este instante de tiempo
	
	#pragma omp parallel for schedule(runtime) private(j,k)
	for(int i = 0; i < N; i++){						//Calculamos la matriz resultante
		for(int j = 0; j < N; j++){
			for(int k=0; k < N ; k++)
				A[i][j] += B[i][k] * C[k][j];
		}
	}
	
	tf = omp_get_wtime();		//Cojemos el instante de tiempo de justo cuando termina el algoritmo
	tt = tf - ti;//Obtenemos el tiempo de ejecucion
						
	
	printf("Tiempo(seg.): %11.9f\t / Tamaño matrices: %u\n", tt, N);
								
		
	
	printf("Matriz Resultante[0] = %f  ", A[0][0]);		 
	printf("Matriz Resultante[%d] = %f  ", N - 1, A[N - 1][N - 1]);		
	printf("\n");
	
	
	
	for(int i = 0; i < N; i++){			//Liberación del espacio cuando usamos matrices dinamicas
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}	
	
	free(A); free(B); free(C);
	
	    
    return 0;
}
