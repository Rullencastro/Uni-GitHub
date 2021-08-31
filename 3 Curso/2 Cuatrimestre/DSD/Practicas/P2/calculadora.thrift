struct matriz{
	1: i32 fil,
  	2: i32 col,
  	3: list<i32> datos,
}


exception InvalidOperation {
   1: string operacion,
   2: string why,
}

service Calculadora{
   i32 sumar(1:i32 num1, 2:i32 num2),
   i32 restar(1:i32 num1, 2:i32 num2),
   i32 multiplicar(1:i32 num1, 2:i32 num2),
   i32 dividir(1:i32 num1, 2:i32 num2) throws(1:InvalidOperation ouch),
   matriz sumarMatriz(1:matriz num1, 2:matriz num2) throws(1:InvalidOperation ouch),
   matriz restarMatriz(1:matriz num1, 2:matriz num2) throws(1:InvalidOperation ouch),
   matriz multiplicarMatriz(1:matriz num1, 2:matriz num2) throws(1:InvalidOperation ouch),
}
