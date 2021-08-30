import java.io.IOException;
import java.net.Socket;
import java.util.Random;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStreamReader;

public class Procesador extends Thread {

    private static final int MAX = 100;
    private Socket socketServicio;
    private int numAdivinar;
    private Random random;
    private int nIntentos;
    public Boolean acertado,identificado;
    private int thread;
    Autentificacion auth = new Autentificacion();

    
    public Procesador(Socket socketServicio,int thread) {
        this.socketServicio = socketServicio;
        this.random = new Random();
        this.thread = thread;

        // Generamos el número a adivinar con random.nextInt(int n), función la cual
        // genera un número
        // entre el intervalo [0,n-1].
        
        do { this.numAdivinar = random.nextInt(MAX); } while (numAdivinar == 0);
        

        nIntentos = 3;
        acertado = false;
    }

    public void run() {
        // Creamos un entero que sera el enviado por el cliente
        int numRecibido,op;
        String user;

        try {
            // Obtiene los flujos de escritura/lectura utilizando PrintWriter y
            // BufferedReader

            BufferedReader inReader = new BufferedReader(new InputStreamReader(socketServicio.getInputStream()));
            PrintWriter outPrint = new PrintWriter(socketServicio.getOutputStream(), true);

            
            outPrint.println("Bienvenido a ADIVINA EL NÚMERO");
            do{
            do{
                outPrint.println("Para poder jugar necesitas identificarte");
                outPrint.println("Elija una opción:\t0 - Iniciar Sesión\t1 - Registrarse");
                op = Integer.parseInt(inReader.readLine());
                if(op!=0 && op!=1){
                    outPrint.println("Opción inválida."); 
                    outPrint.println("false");    
                }else{ 
                    outPrint.println("Opción válida."); 
                    outPrint.println("true");
                }             
            }while(op!=0 && op!=1);
        
            if(op==0)
            {
                outPrint.println("Introduzca su nombre de usuario: ");
                user=inReader.readLine();
                outPrint.println("Introduzca su contraseña: ");
                String password=inReader.readLine();
                identificado= auth.login(user,password);
                if(identificado){
                    outPrint.println("Ha iniciado sesión con éxito");
                    outPrint.println("true");
                }
                else{
                    outPrint.println("Error en el inicio de sesion");
                    outPrint.println("false");
                }
            }
            else{
                outPrint.println("Introduzca el nombre de usuario que desea tener: ");
                user = inReader.readLine();
                outPrint.println("Introduzca la contraseña que desea tener: ");  
                String password = inReader.readLine();
                identificado= auth.newUser(user,password);
                if(identificado){
                    outPrint.println("true");
                    outPrint.println("Ha sido registrado con éxito");
                }
                else{
                    outPrint.println("false");
                    outPrint.println("Error en el registro");
                }
            }
            }while(!identificado);
            do {
                

                // Lee el número que el cliente ha introducido
                numRecibido = Integer.parseInt(inReader.readLine());
                
                // El metodo juego se encarga de procesar si el numero es el correcto,
                // devolviendo mensajes con pistas o si se ha acertado en su caso
                String mensaje = juegoAdivinar(numRecibido);
                System.out.println("Numero recibido: " + numRecibido+" por parte del usuario "+user);
                // Enviamos el mensaje del juego al cliente
                outPrint.println(mensaje);

                // Con esto decimos al cliente si ha cabado el juego o no
                if (acertado)
                    outPrint.println("fin");
                else
                    outPrint.println("nofin");
            } while (!acertado);

        } catch (IOException e) {
            System.err.println("Error al obtener los flujos de entrada/salida.");
        }

    }

    private String juegoAdivinar(int numRecibido) {
        String mensaje = "";
        // Comprueba si el numero recibido es igual al que hay que adivinar,
        // en caso de no serlo, se le otorga una pista y se resta en 1 el numero de
        // intentos.
        if (numRecibido == numAdivinar) {
            mensaje = new String("¡Enhorabuena, has acertado el numero!");
            acertado = true;
        } else {
            mensaje = new String("Lo siento, has fallado. Nºintentos restantes: " + nIntentos + ".");
            // Dependiendo del número de intentos, te da unas pistas u otras
            switch (nIntentos) {
                // Primera pista: dice si el numero es mayor que 75,
                // menor que 25 o esta entre 25 y 75.
                case 3:
                    if (numAdivinar < 25)
                        mensaje = mensaje + " Pista: El numero es menor que 25";
                    else if (numAdivinar > 75)
                        mensaje = mensaje + " Pista: El numero es mayor que 75";
                    else if (numAdivinar >= 25 && numAdivinar <= 75)
                        mensaje = mensaje + " Pista: El numero es mayor que 25 y menor que 75";
                    break;

                // Segunda pista: dice si el numero es múltiplo de 2,3,5,7
                // o no es múltiplo de ninguno de estos.

                case 2:
                    if (numAdivinar % 2 == 0)
                        mensaje = mensaje + " Pista: El numero es multiplo de 2";
                    else if (numAdivinar % 3 == 0)
                        mensaje = mensaje + " Pista: El numero es multiplo de 3";
                    else if (numAdivinar % 5 == 0)
                        mensaje = mensaje + " Pista: El numero es multiplo de 5";
                    else if (numAdivinar % 7 == 0)
                        mensaje = mensaje + " Pista: El numero es multiplo de 7";
                    else if (numAdivinar % 2 != 0 && numAdivinar % 3 != 0)
                        mensaje = mensaje + " Pista: El numero no es multiplo de 2 y 3";
                    else if (numAdivinar % 5 != 0 && numAdivinar % 7 != 0)
                        mensaje = mensaje + " Pista: El numero no es multiplo de 5 y 7";

                    break;

                // Tercera pista: dice en que rango de como máximo 10 valores,
                //en los que se encuentra el número a adivinar
                case 1:
                    if ((numAdivinar - 5) > 0 && (numAdivinar + 5) < 99) {
                        int inf, sup, variacion, op;
                        variacion = random.nextInt(5);
                        op = random.nextInt(2);
                        if (op == 0) {
                            inf = numAdivinar - 5 - variacion;
                            if(inf < 0)
                                inf = 0;
                            sup = numAdivinar + 5 - variacion;

                        } else {
                            inf = numAdivinar - 5 + variacion;
                            sup = numAdivinar + 5 + variacion;
                            if (sup >99)
                                sup = 99;
                        }
                        mensaje = mensaje + " Ultima pista: El numero esta entre " + inf + " y " + sup;

                    } else if ((numAdivinar - 5) <= 0)
                        mensaje = mensaje + " Ultima pista: El numero esta entre 0 y 10";
                    else if ((numAdivinar + 5) >= 99)
                        mensaje = mensaje + " Ultima pista: El numero esta entre 89 y 99";
                    break;

                case 0:
                    mensaje = mensaje + " Te has quedado sin intentos, el numero era " + numAdivinar
                            + ", el servidor gana";
                    acertado = true;
                    break;

            }
            nIntentos--;
        }
        return mensaje;
    }
}
