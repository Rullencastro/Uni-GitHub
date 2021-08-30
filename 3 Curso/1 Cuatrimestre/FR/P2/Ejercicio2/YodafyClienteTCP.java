
//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class YodafyClienteTCP {

	public static void main(String[] args) {
		
		//Cambiamos los buffer de bits por Strings
		String buferEnvio;
		String buferRecepcion;
		
		// Nombre del host donde se ejecuta el servidor:
		String host="localhost";
		// Puerto en el que espera el servidor:
		int port=8989;
		
		// Socket para la conexión TCP
		Socket socketServicio=null;
		
		try {
			// Creamos un socket que se conecte a "host" y "port":
			//////////////////////////////////////////////////////
			socketServicio = new Socket(host,port);
			//////////////////////////////////////////////////////			
			
			//Creacion del buffer de lectura desde el socket llamado inputStream
			BufferedReader inputStream = new BufferedReader(new InputStreamReader (socketServicio.getInputStream()));
			
			//Creación del buffer de envio desde el socket llamado outputStream
			PrintWriter outputStream = new PrintWriter(socketServicio.getOutputStream(),true);

			//Cadena de caracteres que se va a enviar
			buferEnvio = new String ("Al monte del volcan debes ir sin demora");
			
			// Enviamos el array por el outputStream;
			//////////////////////////////////////////////////////
			outputStream.println(buferEnvio);
			//////////////////////////////////////////////////////
			
			// Leemos la respuesta del servidor. 
			//////////////////////////////////////////////////////
			buferRecepcion = inputStream.readLine();
			//////////////////////////////////////////////////////
			
			// Mostremos la cadena de caracteres recibida:
			System.out.println("Recibido:");
			System.out.println(buferRecepcion);
			
			// Una vez terminado el servicio, cerramos el socket (automáticamente se cierran
			// el inputStream  y el outputStream)
			//////////////////////////////////////////////////////
			socketServicio.close();
			//////////////////////////////////////////////////////
			
			// Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
