struct matriz{
	u_int fil;
	u_int col;
	float mat<>;
};

union matriz_response switch(int err) {
	case 0:
		matriz result;
	default:
		void;
};

program CALCULADORA_MAT {
	version CALC_MAT_V1 {
	matriz_response SUMA_MATRICES(matriz,matriz) = 1;
	matriz_response RESTA_MATRICES(matriz,matriz) = 2;
	matriz_response MULT_MATRICES(matriz,matriz) = 3;
	} = 1;
} = 0x2200001;
