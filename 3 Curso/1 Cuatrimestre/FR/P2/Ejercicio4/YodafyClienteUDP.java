//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
import java.io.IOException;
import java.net.UnknownHostException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class YodafyClienteUDP {

	public static void main(String[] args) {
		
		byte []buferEnvio=new byte[256];
		byte []buferRecepcion=new byte[256];
		int bytesLeidos=0;

		//Direccion IP del servidor a buscar
		InetAddress ip;

		//paquete UDP
		DatagramPacket datosPacket;
		
		// Nombre del host donde se ejecuta el servidor:
		String host="localhost";
		// Puerto en el que espera el servidor:
		int port=8989;
		

		// Socket para la conexión UDP
		DatagramSocket socketServicio;
		
		try {
			// Creamos un socket que se conecte a "host" y "port":
			//////////////////////////////////////////////////////
			socketServicio = new DatagramSocket();
			ip =InetAddress.getByName(host);
			//////////////////////////////////////////////////////			
			
		
			
			//Pasamos el buffer de caracteres a un array de bytes:
			buferEnvio ="Al monte del volcan debes ir sin demora".getBytes();
			


			//Creamos el paquete con el buffer con el port y direccion IP correspondiente
			datosPacket= new DatagramPacket(buferEnvio,buferEnvio.length,ip,port);

			//Enviamos este paquete
			socketServicio.send(datosPacket);

			//creamos el paquete para recibir los datos con el port y direccion IP correspondiente
			datosPacket=new DatagramPacket(buferRecepcion,buferRecepcion.length,ip,port);
			socketServicio.receive(datosPacket);
			
	
			// MOstremos la cadena de caracteres recibidos:
			System.out.println("Recibido: ");
			for(int i=0;i<datosPacket.getData().length;i++){
				System.out.print((char)datosPacket.getData()[i]);
			}
			
			// Una vez terminado el servicio, cerramos el socket (automáticamente se cierran
			// el inpuStream  y el outputStream)
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
