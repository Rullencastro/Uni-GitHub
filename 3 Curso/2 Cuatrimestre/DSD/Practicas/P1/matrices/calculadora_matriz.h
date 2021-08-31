/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CALCULADORA_MATRIZ_H_RPCGEN
#define _CALCULADORA_MATRIZ_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct matriz {
	u_int fil;
	u_int col;
	struct {
		u_int mat_len;
		float *mat_val;
	} mat;
};
typedef struct matriz matriz;

struct matriz_response {
	int err;
	union {
		matriz result;
	} matriz_response_u;
};
typedef struct matriz_response matriz_response;

struct suma_matrices_1_argument {
	matriz arg1;
	matriz arg2;
};
typedef struct suma_matrices_1_argument suma_matrices_1_argument;

struct resta_matrices_1_argument {
	matriz arg1;
	matriz arg2;
};
typedef struct resta_matrices_1_argument resta_matrices_1_argument;

struct mult_matrices_1_argument {
	matriz arg1;
	matriz arg2;
};
typedef struct mult_matrices_1_argument mult_matrices_1_argument;

#define CALCULADORA_MAT 0x2200001
#define CALC_MAT_V1 1

#if defined(__STDC__) || defined(__cplusplus)
#define SUMA_MATRICES 1
extern  matriz_response * suma_matrices_1(matriz , matriz , CLIENT *);
extern  matriz_response * suma_matrices_1_svc(matriz , matriz , struct svc_req *);
#define RESTA_MATRICES 2
extern  matriz_response * resta_matrices_1(matriz , matriz , CLIENT *);
extern  matriz_response * resta_matrices_1_svc(matriz , matriz , struct svc_req *);
#define MULT_MATRICES 3
extern  matriz_response * mult_matrices_1(matriz , matriz , CLIENT *);
extern  matriz_response * mult_matrices_1_svc(matriz , matriz , struct svc_req *);
extern int calculadora_mat_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SUMA_MATRICES 1
extern  matriz_response * suma_matrices_1();
extern  matriz_response * suma_matrices_1_svc();
#define RESTA_MATRICES 2
extern  matriz_response * resta_matrices_1();
extern  matriz_response * resta_matrices_1_svc();
#define MULT_MATRICES 3
extern  matriz_response * mult_matrices_1();
extern  matriz_response * mult_matrices_1_svc();
extern int calculadora_mat_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_matriz (XDR *, matriz*);
extern  bool_t xdr_matriz_response (XDR *, matriz_response*);
extern  bool_t xdr_suma_matrices_1_argument (XDR *, suma_matrices_1_argument*);
extern  bool_t xdr_resta_matrices_1_argument (XDR *, resta_matrices_1_argument*);
extern  bool_t xdr_mult_matrices_1_argument (XDR *, mult_matrices_1_argument*);

#else /* K&R C */
extern bool_t xdr_matriz ();
extern bool_t xdr_matriz_response ();
extern bool_t xdr_suma_matrices_1_argument ();
extern bool_t xdr_resta_matrices_1_argument ();
extern bool_t xdr_mult_matrices_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_CALCULADORA_MATRIZ_H_RPCGEN */
