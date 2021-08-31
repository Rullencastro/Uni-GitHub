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
    
    double ti,tf,tt;			//Definimos estructuras para medir el tiempo
    

    unsigned int N = atoi(argv[1]);


        double **matriz, *vector, *vr;			//Inicializacion dinámica de la matriz y los vectores
        matriz = (double**) malloc(N * sizeof(double*));
        for(int i = 0; i < N; ++i)
            matriz[i] = (double*) malloc(N * sizeof(double));

        vector = (double*) malloc(N * sizeof(double));
        vr = (double*) malloc(N * sizeof(double));
       
 
    
    for(int i = 0; i < N; ++i)
    {					//Se rellenan el vector y la matriz con valores
        for(int j = 0; j < N; ++j)
	{
		if(j >= i)
        		matriz[i][j] = 6.12345;
		else
			matriz[i][j] = 0;
	}
	vector[i] = 2.321;
	vr[i] = 0;
    }
	
	
	ti = omp_get_wtime();		//Cojemos este instante de tiempo
	
	#pragma omp parallel for schedule(runtime)
	for(int i = 0; i < N; i++)
	{			//Calculamos el vector resultante
		for(int j = i; j < N; j++)
			vr[i] += matriz[i][j] * vector[i];
	}
	
	tf = omp_get_wtime();		//Cojemos el instante de tiempo de justo cuando termina el algoritmo
	tt = tf - ti; //Obtenemos el tiempo de ejecucion
						

	
	
	printf("Tiempo(seg.): %11.9f\t / Tamaño vectores y matriz: %u\n", tt, N);
	if(N < 15)								//Con este if/else mostramos el vector con valores pequeños
		for(int i = 0; i < N; i++){
			printf("Vector Resultante[%d] = %f  ", i, vr[i]);
			printf("\n");
		}
	else{
		printf("Vector Resultante[0] = %f  ", vr[0]);
		printf("Vector Resultante[%d] = %f  ", N - 1, vr[N - 1]);
		printf("\n");
	}
	
	
	for(int i = 0; i < N; i++)			//Liberación del espacio cuando usamos matriz y vectores dinamicos
		free(matriz[i]);	
	
	free(matriz); free(vector); free(vr);
	
	    
    return 0;
}
