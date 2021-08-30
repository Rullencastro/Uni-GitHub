//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Random;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStreamReader;


//
// Nota: si esta clase extendiera la clase Thread, y el procesamiento lo hiciera el método "run()",
// ¡Podríamos realizar un procesado concurrente! 
//
public class ProcTCPConcurrente extends Thread {
	// Referencia a un socket para enviar/recibir las peticiones/respuestas
	private Socket socketServicio;
	
	// Para que la respuesta sea siempre diferente, usamos un generador de números aleatorios.
	private Random random;
	
	private int thread;

	// Constructor que tiene como parámetro una referencia al socket abierto en por otra clase
	public ProcTCPConcurrente(Socket socketServicio, int thread) {
		this.socketServicio=socketServicio;
		random=new Random();
		this.thread = thread;
	}

	
	// Aquí es donde se realiza el procesamiento realmente:
	public void run(){
		
		//Creamos un string
		String cadenaRecibida=new String();
	
		try {
			// Obtiene los flujos de escritura/lectura utilizando PrintWriter y BufferedReader

			BufferedReader inReader= new BufferedReader(new InputStreamReader(socketServicio.getInputStream()));
			PrintWriter outPrint= new PrintWriter(socketServicio.getOutputStream(),true); 
			
			// Lee la frase a Yodaficar:
			////////////////////////////////////////////////////////
			cadenaRecibida = inReader.readLine();
			////////////////////////////////////////////////////////
			
			// Yoda hace su magia:
			// Yoda reinterpreta el mensaje:
			String recepcion=yodaDo(cadenaRecibida);
					
			// Enviamos la traducción de Yoda:
			////////////////////////////////////////////////////////
			System.out.println("Cadena enviada por thread "+this.thread);
			outPrint.println(recepcion);
			System.out.println("Cadena devuelta por thread "+this.thread);
			////////////////////////////////////////////////////////
			
			
		} catch (IOException e) {
			System.err.println("Error al obtener los flujos de entrada/salida.");
		}

	}

	// Yoda interpreta una frase y la devuelve en su "dialecto":
	private String yodaDo(String peticion) {
		// Desordenamos las palabras:
		String[] s = peticion.split(" ");
		String resultado="";
		
		for(int i=0;i<s.length;i++){
			int j=random.nextInt(s.length);
			int k=random.nextInt(s.length);
			String tmp=s[j];
			
			s[j]=s[k];
			s[k]=tmp;
		}
		
		resultado=s[0];
		for(int i=1;i<s.length;i++){
		  resultado+=" "+s[i];
		}
		
		return resultado;
	}
}
