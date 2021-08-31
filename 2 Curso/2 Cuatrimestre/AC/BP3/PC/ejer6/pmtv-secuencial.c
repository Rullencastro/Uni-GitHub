#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>



int main(int argc, char const *argv[]){			
	if(argc != 2){					//Esto es para aceptar el argumento N(numero de filas/columnas)
        printf("Error de argumentos %s", argv[0]);
        return(EXIT_FAILURE);
    }
    
    struct timespec cgt1, cgt2;			//Definimos estructuras para medir el tiempo
    double ncgt;

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

	clock_gettime(CLOCK_REALTIME, &cgt1);		//Cojemos este instante de tiempo
	
	for(int i = 0; i < N; i++)
	{			//Calculamos el vector resultante
		for(int j = i; j < N; j++)
			vr[i] += matriz[i][j] * vector[i];
	}
	
	clock_gettime(CLOCK_REALTIME, &cgt2);		//Cojemos el instante de tiempo de justo cuando termina el algoritmo
	ncgt = (double) (cgt2.tv_sec - cgt1.tv_sec) + (double) (cgt2.tv_nsec - cgt1.tv_nsec) / (1.e+9); //Obtenemos el tiempo de ejecucion
						

	printf("Matriz trinagular superior: \n");
	for(int i = 0; i < N; ++i){					
        	for(int j = 0; j < N; ++j)
			printf("%f ",matriz[i][j]);
		printf("\n");
	}

	printf("Vector que va a multiplicar a la matriz: \n");
	for(int i = 0; i < N; ++i)				
		printf("%f ",vector[i]);
	printf("\n");


	printf("Vector resultante: \n");
	for(int i = 0; i < N; ++i)					
		printf("%f ",vr[i]);
	printf("\n");
	
	
	
	printf("Tiempo(seg.): %11.9f\t / Tamaño vectores y matriz: %u\n", ncgt, N);
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
