

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
public class ServerTCPConcurrente {

	public static void main(String[] args) {
	
		// Puerto de escucha
		int port=8989;
		
		//número de hebras/threads
		int thread=0;

		ServerSocket serverSocket;
		Socket socketServicio = null;
		try {
			// Abrimos el socket en modo pasivo, escuchando el en puerto indicado por "port"
			//////////////////////////////////////////////////
			serverSocket = new ServerSocket(port);
			//////////////////////////////////////////////////
			
			// Mientras ... siempre!
			do {
				
				// Aceptamos una nueva conexión con accept()
				/////////////////////////////////////////////////
				try{
					socketServicio = serverSocket.accept();
				}catch (IOException e){
					System.err.println("Error: no se pudo aceptar la conexión solicitada");
				}
				//////////////////////////////////////////////////
				
				// Creamos un objeto de la clase ProcesadorYodafy, pasándole como 
				// argumento el nuevo socket, para que realice el procesamiento
				// Este esquema permite que se puedan usar hebras más fácilmente.
				
				thread++; //Aumentamos el número de hebras cada vez que llega una petición de conexión
				System.out.println("Numero de hebras " + thread);
				ProcTCPConcurrente procesador=new ProcTCPConcurrente(socketServicio,thread);
				procesador.start();
				
			} while (true);
			
		} catch (IOException e) {
			System.err.println("Error al escuchar en el puerto "+port);
		}

	}

}
