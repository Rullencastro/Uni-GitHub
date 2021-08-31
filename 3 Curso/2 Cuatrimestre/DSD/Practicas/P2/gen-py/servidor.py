from calculadora import Calculadora
from calculadora.ttypes import matriz,InvalidOperation

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

import logging

logging.basicConfig(level=logging.DEBUG)

class CalculadoraHandler:
    def __init__(self):
        self.log = {}

    def sumar(self, num1, num2):
        return num1 + num2

    def restar(self, num1, num2):
        return num1 - num2

    def multiplicar(self, num1, num2):
        return num1 * num2

    def dividir(self, num1, num2):
        if num2 == 0:
            raise InvalidOperation("División", "No se puede dividir entre 0")
        else:
            return num1 // num2

    def sumarMatriz(self, num1, num2):
        if (num1.fil != num2.fil) or (num1.col != num2.col):
            raise InvalidOperation("Suma de matrices", "Las matrices deben ser de las mismas dimensiones")
        else:
            result = matriz()
            result.fil = num1.fil
            result.col = num1.col
            result.datos = []
            i = 0
            dimension = num1.fil*num1.col
            while i < dimension:
                result.datos.append(num1.datos[i] + num2.datos[i])
                i = i + 1
            return result

    def restarMatriz(self, num1, num2):
        if (num1.fil != num2.fil) or (num1.col != num2.col):
            raise InvalidOperation("Resta de matrices", "Las matrices deben ser de las mismas dimensiones")
        else:
            result = matriz()
            result.fil = num1.fil
            result.col = num1.col
            result.datos = []
            i = 0
            dimension = num1.fil*num1.col
            while i < dimension:
                result.datos.append(num1.datos[i] - num2.datos[i])
                i = i + 1
            return result

    def multiplicarMatriz(self, num1, num2):
        if (num1.col != num2.fil):
            raise InvalidOperation("Multiplicación de matrices", "El número de Columnas de la primera Matriz debe coincidir con el número de Filas de la segunda Matriz")
        else:
            result = matriz()
            result.fil = num1.fil
            result.col = num2.col
            result.datos = []
            dimension = result.fil * result.col
            i = 0
            while i < result.fil:
                j = 0
                while j < result.col:
                    suma = 0
                    k = 0
                    while k < num2.fil:
                        suma += num1.datos[num1.fil*i+k] * num2.datos[num2.col*k+j]
                        k = k + 1
                    result.datos.append(suma)
                    j = j + 1
                i = i + 1

            return result


        
if __name__ == "__main__":
    handler = CalculadoraHandler()
    processor = Calculadora.Processor(handler)
    transport = TSocket.TServerSocket(host="127.0.0.1", port=9090)
    tfactory = TTransport.TBufferedTransportFactory()
    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

    print("iniciando servidor...")
    server.serve()        
