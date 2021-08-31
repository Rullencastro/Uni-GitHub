/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculadora_matriz.h"
#include <errno.h>

void
calculadora_1(char *host,char op,matriz a,matriz b)
{
	CLIENT *clnt;
	matriz_response *result;
	


#ifndef	DEBUG
	clnt = clnt_create (host, CALCULADORA_MAT, CALC_MAT_V1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	switch(op){
	
		case '+':
		result = suma_matrices_1(a,b,clnt);
		break;
		case '-':
		result = resta_matrices_1(a,b,clnt);
		break;
		case '*': 
		result = mult_matrices_1(a,b,clnt);
		break;
	}
	if (result == (matriz_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	if (result->err != 0) {
		errno = result->err;
		perror("Ha habido algún error en la operación");
		exit(1);
	} 

	printf("Imprimiendo el resultado\n");
    for (int i = 0; i < result->matriz_response_u.result.fil; i++) {
        for (int j = 0; j < result->matriz_response_u.result.col; j++) {
			printf("%f ", *(result->matriz_response_u.result.mat.mat_val+i*result->matriz_response_u.result.col+j));
            // printf("%f ", result->mat.mat_val[result->fil*i+j]);
        }
        printf("\n");
    }


	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	char op;
	matriz a,b;

	if (argc < 5) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	
	op = *argv[2];
	a.fil = atoi(argv[3]);
	a.col = atoi(argv[4]);
	a.mat.mat_len= a.col*a.fil;
	a.mat.mat_val = malloc(a.mat.mat_len*sizeof(float));
	for(int i = 0;i < a.mat.mat_len;i++){
		a.mat.mat_val[i] = atof(argv[5+i]);
	}

	b.fil = atoi(argv[5+a.mat.mat_len]);
	b.col = atoi(argv[6+a.mat.mat_len]);
	b.mat.mat_len= b.col*b.fil;
	b.mat.mat_val = malloc(b.mat.mat_len*sizeof(float));
	for(int i = 0;i < b.mat.mat_len;i++){
		b.mat.mat_val[i] = atof(argv[7+a.mat.mat_len+i]);
	}


	host = argv[1];
	calculadora_1 (host,op,a,b);

	free(a.mat.mat_val);
	free(b.mat.mat_val);
exit (0);
}
