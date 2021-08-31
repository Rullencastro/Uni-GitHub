
union calculadora_response switch(int err) {
	case 0:
		float result;
	default:
		void;
};

program CALCULADORA {
	version CALCV1 {
	calculadora_response SUMAR (float,float) = 1;
	calculadora_response RESTAR (float,float) = 2;
	calculadora_response MULTIPLICAR (float,float) = 3;
	calculadora_response DIVIDIR (float,float) = 4;
	} = 1;
} = 0x20000001;
