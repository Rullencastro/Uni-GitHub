/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "calculadora_matriz.h"

bool_t
xdr_matriz (XDR *xdrs, matriz *objp)
{
	register int32_t *buf;

	 if (!xdr_u_int (xdrs, &objp->fil))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->col))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->mat.mat_val, (u_int *) &objp->mat.mat_len, ~0,
		sizeof (float), (xdrproc_t) xdr_float))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_matriz_response (XDR *xdrs, matriz_response *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case 0:
		 if (!xdr_matriz (xdrs, &objp->matriz_response_u.result))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_suma_matrices_1_argument (XDR *xdrs, suma_matrices_1_argument *objp)
{
	 if (!xdr_matriz (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_matriz (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_resta_matrices_1_argument (XDR *xdrs, resta_matrices_1_argument *objp)
{
	 if (!xdr_matriz (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_matriz (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mult_matrices_1_argument (XDR *xdrs, mult_matrices_1_argument *objp)
{
	 if (!xdr_matriz (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_matriz (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}
