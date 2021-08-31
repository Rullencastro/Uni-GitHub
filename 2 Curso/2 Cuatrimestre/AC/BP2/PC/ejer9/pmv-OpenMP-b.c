#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <omp.h>

//#define GLOBAL
#define DINAMIC

#ifdef GLOBAL
	#define MAX 33554432
#endif

int main(int argc, char const *argv[]){			
	if(argc != 2){					//Esto es para aceptar el argumento N(numero de filas/columnas)
        printf("Error de argumentos %s", argv[0]);
        return(EXIT_FAILURE);
    }
    
    struct timespec cgt1, cgt2;			//Definimos estructuras para medir el tiempo
    double ncgt;

    int N = atoi(argv[1]);

    #ifdef GLOBAL				//Se inicializan los vectores y la matriz de forma global
    	if(N > MAX) N = MAX;
        int matriz[N][N];
        int vector[N];
        int vr[N];
        printf("Con Vectores Globales\n");
    #endif

    #ifdef DINAMIC
        int **matriz, *vector, *vr;			//Inicializacion dinámica de la matriz y los vectores
        matriz = (int**) malloc(N * sizeof(int*));
        for(int i = 0; i < N; ++i)
            matriz[i] = (int*) malloc(N * sizeof(int));
       
        vector = (int*) malloc(N * sizeof(int));
        vr = (int*) malloc(N * sizeof(int));
        printf("Con Vectores Dinámicos\n");
    #endif

    #pragma omp parallel for 
    for(int i = 0; i < N; ++i){				//Se rellenan el vector y la matriz con valores
    	vector[i] = i;
        for(int j = 0; j < N; ++j)
        	matriz[i][j] = i + j;
	}

	clock_gettime(CLOCK_REALTIME, &cgt1);		//Cojemos este instante de tiempo
	
	
	for(int i = 0; i < N; i++){			//Calculamos el vector resultante
		int suma = 0;
		#pragma omp parallel for 
		for(int j = 0; j < N; j++)
			#pragma omp atomic
			suma += matriz[i][j] * vector[j];
			
		vr[i] = suma;
	}
	
	clock_gettime(CLOCK_REALTIME, &cgt2);		//Cojemos el instante de tiempo de justo cuando termina el algoritmo
	ncgt = (double) (cgt2.tv_sec - cgt1.tv_sec) + (double) (cgt2.tv_nsec - cgt1.tv_nsec) / (1.e+9); //Obtenemos el tiempo de ejecucion
							
	printf("Tiempo(seg.): %11.9f\t / Tamaño vectores: %u\n", ncgt, N);
	if(N < 15)								//Con este if/else mostramos el vector con valores pequeños
		for(int i = 0; i < N; i++){
			printf("Vector Resultante[%d] = %d  ", i, vr[i]);
			printf("\n");
		}
	else{
		printf("Vector Resultante[0] = %d  ", vr[0]);
		printf("Vector Resultante[%d] = %d  ", N - 1, vr[N - 1]);
		printf("\n");
	}
	
	#ifdef DINAMIC
	for(int i = 0; i < N; i++)			//Liberación del espacio cuando usamos matriz y vectores dinamicos
		free(matriz[i]);	
	
	free(matriz); free(vector); free(vr);
	#endif
	    
    return 0;
}
