import java.io.FileWriter;
import java.io.File;
import java.util.Scanner;
import java.util.Map;
import java.util.HashMap;

public class Autentificacion {
    File baseDatos; //Fichero que va a actuar como base de datos, guardando todos los usuarios registrados en el servidor
    Map<String, String> users_pass; //Estructura de datos utilizada para guardar los datos de un usuario NOMBRE_USUARIO-CONTRASEÑA
    FileWriter escritura; //Escribe en la base de datos el usuario registrado
    Scanner reader; 

    public Autentificacion(){
        this.users_pass = new HashMap<String, String>();
        this.baseDatos = new File("./baseDeDatos.txt");
        this.escritura= null;
        this.reader=null;
        leerBaseDeDatos();
    }


    //Metodo que lee la base de datos para tener a todos los usuarios registrados del servidor
    private void leerBaseDeDatos()
    {
        try {
            // Leemos el contenido de la base de datos
            reader = new Scanner(baseDatos);
            String[] s;

			while (reader.hasNextLine()) {
                s = reader.nextLine().split(" ");
                String user = s[0]; 	
                String password = s[1];
                users_pass.put(user, password);
            }
        } catch (Exception error) {
            System.out.println("Error: "+error.getMessage());
            }
            // Cerramos el fichero 
		try {
            if (reader != null)
                reader.close();
            } catch (Exception error2) {
				System.out.println("Error: " + error2.getMessage());
			}
    }

    
//Metodo que registra a un nuevo usuario en la base de datos
public Boolean newUser(String user,String password)
{
    //Primero comprobamos si el usuario ya existe en la base de datos
    if(users_pass.containsKey(user)){
        System.out.println("Lo siento, el usuario ya existe.");
        return false;
    }
    else{
        //Si no existe, lo registramos o mejor dicho, lo guardamos en la base de datos
        users_pass.put(user, password);
        escribirBaseDeDatos(user, password);
        return true;
    }
}

private void escribirBaseDeDatos(String user,String password)
{
    
    try{
        //Cargamos el fichero de escritura
        escritura= new FileWriter("./baseDeDatos.txt",true);
        //Escribimos en el fichero los datos con formato user password
        escritura.write(user + " "+password + "\n");

		escritura.close();

	} catch (Exception error) {
			System.out.println("Error: " + error.getMessage());
		}
}

public boolean login(String user,String password)
{
    //Primero comprobamos si el usuario registrado en la base de datos existe
    if(users_pass.containsKey(user))
    {
        //Si existe comprobamos que su contraseña sea igual a la introducida
        String contraseñaVerdadera=users_pass.get(user);
        return contraseñaVerdadera.equals(password);
    }
    else{  
        return false;
    }
}

}