import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.Scanner; 

public class Cliente {
    public static void main(String args[]) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        if (args.length < 2) {
            System.out.println("Error, uso correcto java Servidor (IP) (0|1|2|3|4)");
            System.exit(0);
        }

        try {
            String nombre_objeto_remoto = "";

            if (Integer.parseInt(args[1]) == 0)
                nombre_objeto_remoto = "Original";
            if (Integer.parseInt(args[1]) == 1)
                nombre_objeto_remoto = "Replica1";
            if (Integer.parseInt(args[1]) == 2)
                nombre_objeto_remoto = "Replica2";
            if (Integer.parseInt(args[1]) == 3)
                nombre_objeto_remoto = "Replica3";
            if (Integer.parseInt(args[1]) == 4)
                nombre_objeto_remoto = "Replica4";

            Registry registry = LocateRegistry.getRegistry(args[0]);
            I_Donaciones instancia_local = (I_Donaciones) registry.lookup(nombre_objeto_remoto);

            Scanner scan = new Scanner(System.in);
            boolean salir = false;

            while (!salir) {
                System.out.println(
                        "\nElija que desea hacer\n\t0 - Registrarse\n\t1 - Donar\n\t2 - Consultar total donado\n\t3 - Consultar total local\n\t4 - Consultar usuarios locales\n\t5 - Consultar usuarios globales\n\t6 - Salir");
                int op;
                op = scan.nextInt();
                switch (op) {
                case 0:
                    System.out.println("\nIntroduzca nombre de usuario a registrar");
                    Scanner s = new Scanner(System.in);
                    String a = s.nextLine();
                    instancia_local.registrar(a);
                    break;
                case 1:
                    System.out.println("\nIntroduzca nombre de usuario y cantidad a donar");
                    Scanner s1 = new Scanner(System.in);
                    String a1 = s1.nextLine();
                    int b1 = s1.nextInt();
                    instancia_local.donar(a1, b1);
                    break;
                case 2:
                    System.out.println("\nIntroduzca nombre de usuario");
                    Scanner s2 = new Scanner(System.in);
                    String a2 = s2.nextLine();
                    System.out.println("Total donado(0 = No puedes consultarlo): " + instancia_local.getTotal(a2));
                    break;
                case 3:
                    System.out.println("Total donado localmente(0 = No puedes consultarlo): " + instancia_local.getTotalLocal());
                    break;
                case 4:
                    ArrayList<String> a4 = instancia_local.getUsuarios();
                    System.out.print("Usuarios registrados localmente:");
                    for(int i = 0 ;i < a4.size();i++){
                        System.out.print(" " + a4.get(i));
                    }
                    System.out.println("");
                    break;
                case 5:
                    ArrayList<String> a5 = instancia_local.getUsuariosTotales();
                    System.out.print("Usuarios registrados:");
                    for(int i = 0 ;i < a5.size();i++){
                        System.out.print(" " + a5.get(i));
                    }
                    System.out.println("");
                    break;
                case 6:
                    salir = true;
                    System.out.println("\nPase buen día");
                    break;
                }
            }
            scan.close();

        } catch (Exception e) {
            System.err.println("Error al invocar el servidor");
            e.printStackTrace();
        }
    }
}
