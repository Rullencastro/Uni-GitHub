package gen;

import org.apache.thrift.TException;
import org.apache.thrift.transport.TSSLTransportFactory;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TSSLTransportFactory.TSSLTransportParameters;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

import java.util.Arrays;

public class JavaCliente {
    public static void main(String [] args) {

        try {
            TTransport transport;
            transport = new TSocket("localhost", 9090);
            transport.open();


            TProtocol protocol = new  TBinaryProtocol(transport);
            Calculadora.Client cliente = new Calculadora.Client(protocol);

            perform(cliente);

            transport.close();
        } catch (TException x) {
            x.printStackTrace();
        }
    }

    private static void imprimir_matriz(matriz m){
        for(int i = 0;i < m.fil;i++){
            for(int j = 0;j < m.col;j++){
                System.out.print(m.datos.get(m.col * i + j) +" ");
            }
            System.out.println(" ");
        }
    }

    private static void perform(Calculadora.Client cliente) throws TException
    {
        System.out.println("Operaciones básicas");

        int sum = cliente.sumar(6,2);
        System.out.println("6 + 2 = " + sum);

        int res = cliente.restar(6,2);
        System.out.println("6 - 2 = " + res);

        int mul = cliente.multiplicar(6,2);
        System.out.println("6 * 2 = " + mul);

        int div = cliente.dividir(6,2);
        System.out.println("6 / 2 = " + div);

        matriz a = new matriz();
        a.fil = 3;
        a.col = 3;
        a.datos = Arrays.asList(1,2,3,4,5,6,7,8,9);
        matriz a2 = new matriz();
        a2.fil = 3;
        a2.col = 2;
        a2.datos = Arrays.asList(1,2,3,4,5,6);


        System.out.println("Operaciones con matrices");
        System.out.println("Matriz resultado de sumarlas: ");
        matriz b = cliente.sumarMatriz(a,a);
        imprimir_matriz(b);

        System.out.println("Matriz resultado de restarlas: ");
        matriz c = cliente.restarMatriz(a,a);
        imprimir_matriz(c);

        System.out.println("Matriz resultado de multiplicarlas: ");
        matriz d = cliente.multiplicarMatriz(a,a);
        imprimir_matriz(d);
    }
}
