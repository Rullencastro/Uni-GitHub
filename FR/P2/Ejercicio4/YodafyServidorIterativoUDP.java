

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
public class YodafyServidorIterativoUDP {

	public static void main(String[] args) {
	
		// Puerto de escucha
		int port=8989;
		// array de bytes auxiliar para recibir o enviar datos.
		byte []buffer=new byte[256];
		// Número de bytes leídos
		int bytesLeidos=0;
		
		DatagramSocket serverSocket;

		DatagramPacket datosPacket;
		try {
			// Abrimos el socket en modo pasivo, escuchando el en puerto indicado por "port"
			//////////////////////////////////////////////////
			serverSocket = new DatagramSocket(port);
			//////////////////////////////////////////////////
			datosPacket = new DatagramPacket(buffer,buffer.length);
			// Mientras ... siempre!
			do {
				
				// Creamos un objeto de la clase ProcesadorYodafy, pasándole como 
				// argumento el nuevo socket, para que realice el procesamiento
				// Este esquema permite que se puedan usar hebras más fácilmente.
				ProcesadorYodafyUDP procesador=new ProcesadorYodafyUDP(serverSocket);
				procesador.procesa();
				
			} while (true);
			
		} catch (IOException e) {
			System.err.println("Error al escuchar en el puerto "+port);
		}

	}

}
