/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculadora.h"

#include <errno.h>

void
calculadora_1(char *host,float a, float b, char op)
{
	CLIENT *clnt;
	calculadora_response* result;

#ifndef	DEBUG
	clnt = clnt_create (host, CALCULADORA, CALCV1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	switch(op){
	
		case '+':
		result = sumar_1(a, b, clnt);
		break;
		case '-':
		result = restar_1(a, b, clnt);
		break;
		case '*': 
		result = multiplicar_1(a, b, clnt);
		break;
		case '/': 
		result = dividir_1(a, b, clnt);
		break;
	}	
	if (result == (calculadora_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	if (result->err != 0) {
		errno = result->err;
		perror("Ha habido algún error en la operación");
		exit(1);
	} 

	printf("Resultado de la operación %f %c %f = %f\n",a,op,b,result->calculadora_response_u.result);

	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	float a,b;
	char op;

	if (argc < 5) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	
	a = atof(argv[2]);
	b = atof(argv[4]);
	op = *argv[3];
	
	host = argv[1];
	calculadora_1 (host,a,b,op);
exit (0);
}
