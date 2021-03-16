import java.io.IOException;
import java.net.Socket;
import java.util.Random;
import java.util.Scanner;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStreamReader;


public class Cliente {

    public static void main(String[] args) {

        String host = "localhost";
        int port = 8990;
        int numEnviar;
        boolean fin = false;
        String cadenaRecibida,f;
        Socket socketServicio = null;
        Scanner reader = new Scanner(System.in);

        Autentificacion auth = new Autentificacion();
        int op;
        Boolean identificado,cont;
        
        try{

            // Creamos un socket que se conecte a "host" y "port":
			socketServicio = new Socket(host,port);

			//Creacion del buffer de lectura desde el socket llamado inputStream
			BufferedReader inReader = new BufferedReader(new InputStreamReader (socketServicio.getInputStream()));
			
			//Creación del buffer de envio desde el socket llamado outputStream
			PrintWriter outPrinter = new PrintWriter(socketServicio.getOutputStream(),true);
           
            
            System.out.println(inReader.readLine());
            do{
                do{
                System.out.println(inReader.readLine());
                System.out.println(inReader.readLine());
                op = reader.nextInt();
                outPrinter.println(op);
                System.out.println(inReader.readLine());
                }while(!Boolean.parseBoolean(inReader.readLine()));
                
           
                if(op==0)
                {
                System.out.println(inReader.readLine());
                String user=reader.next();
                outPrinter.println(user);
                System.out.println(inReader.readLine());
                String password=reader.next();
                outPrinter.println(password);
                System.out.println(inReader.readLine());
                identificado = Boolean.parseBoolean(inReader.readLine());
                }
                else{
                    System.out.println(inReader.readLine());
                    String user = reader.next();
                    outPrinter.println(user);
                    System.out.println(inReader.readLine());
                    String password=reader.next();
                    outPrinter.println(password);
                    identificado= Boolean.parseBoolean(inReader.readLine());
                    System.out.println(inReader.readLine());
                }

            }while(!identificado);

            System.out.println("\n\nADIVINA UN NUMERO DEL 0 AL 99 ");

            do{
                //Lectura de del numero introducido por el cliente
                System.out.println("Introduzca el número:");
                numEnviar = reader.nextInt();

                //Enviamos dicho número al servidor
                outPrinter.println(numEnviar);
                
                //Leemos la respuesta del servidor
                cadenaRecibida = inReader.readLine();
                f=inReader.readLine();
                
                //Mostramos la cadena de caracteres recibida
                System.out.println(cadenaRecibida);

                //si recibimos del servidor la cadena fin significa que el juego a acabado
                if(f.equals("fin"))
                    fin = true;
            
            }while(!fin);

        // Una vez terminado el servicio, cerramos el socket (automáticamente se cierran el inputStream  y el outputStream)
			
			socketServicio.close();

           
        }catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}

    }
}
