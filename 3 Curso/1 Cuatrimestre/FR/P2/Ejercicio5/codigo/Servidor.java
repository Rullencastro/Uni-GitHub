import java.io.IOException;
import java.net.Socket;
import java.net.ServerSocket;

public class Servidor {

    public static void main(String args[]) {
        int port = 8990;

        int thread = 0;

        ServerSocket serverSocket;
        Socket socketServicio = null;

        try {
            serverSocket = new ServerSocket(port);

            do {
                try {
                    socketServicio = serverSocket.accept();
                } catch (IOException e) {
                    System.err.println("Error: no se pudo aceptar la conexión solicitada");
                }

                thread++; //Aumentamos el número de hebras cada vez que llega una petición de conexión
				System.out.println("Numero de hebras " + thread);
                Procesador procesador = new Procesador(socketServicio,thread);
                procesador.start();
            } while (true);

        } catch (IOException e) {
            System.err.println("Error al escuchar en el puerto " + port);
        }
    }
}