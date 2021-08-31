import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
//import java.net.MalformedURLException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;

public class Servidor implements I_Donaciones {
    
    private ArrayList<String> usuarios;
    private double totalLocal;
    private I_Donaciones replica;
    public ArrayList<I_Donaciones> v_replicas;
    private ArrayList<String> registro;

    public Servidor() {
        super();
        totalLocal = 0;
        usuarios = new ArrayList<String>();
        registro = new ArrayList<String>();
        v_replicas = new ArrayList<I_Donaciones>();

    }

    @Override
    public void registrar(String usuario) throws RemoteException {
        boolean existe = false;
        
        for (int i = 0; i < this.v_replicas.size() && !existe; i++) {
            if (this.v_replicas.get(i).getUsuarios().contains(usuario))
                existe = true;
        }

        if (!existe) {
            this.replica = this;
            for (int i = 0; i < this.v_replicas.size(); i++) {
                if (this.replica.getUsuarios().size() > this.v_replicas.get(i).getUsuarios().size())
                    this.replica = this.v_replicas.get(i);
            }

            this.replica.addUsuario(usuario);
        } else
            System.out.println("El usuario " + usuario + " ya esta registrado");
    }

    @Override
    public void donar(String usuario, double cantidad) throws RemoteException {
        if (cantidad > 0) {
            boolean existe = false;

            for (int i = 0; i < this.v_replicas.size() && !existe; i++) {
                if (this.v_replicas.get(i).getUsuarios().contains(usuario)) {
                    existe = true;
                    this.replica = this.v_replicas.get(i);
                }
            }

            if (existe) {
                this.replica.donacion(cantidad);
                this.replica.addDonante(usuario);
            } else
                System.out.println("No puedes donar sin estar registrado");
        } else
            System.out.println("Se ha intentado donar una cantidad no válida");
    }

    @Override
    public double getTotal(String usuario) throws RemoteException {
        double total = 0;
        boolean existe = false;

        for (int i = 0; i < this.v_replicas.size() && !existe; i++) {
            if (this.v_replicas.get(i).getUsuarios().contains(usuario)) {
                existe = true;
                this.replica = this.v_replicas.get(i);
            }
        }

        if (existe) {
            if (this.replica.getRegistro().contains(usuario)) {
                for (int i = 0; i < this.v_replicas.size(); i++) {
                    total += this.v_replicas.get(i).getTotalLocal();
                }
            } else
                System.out.println(
                        "El usuario no puede consultar el total donado hasta que realize como mínimo una donación");
        } else
            System.out.println("El usuario no puede consultar el total donado sin estar registrado antes");

        return total;
    }

    
    public void addReplica(I_Donaciones ref) {
        this.v_replicas.add(ref);
    }
    
    public ArrayList<String> getUsuarios() throws RemoteException {
        return this.usuarios;
    }

    public ArrayList<String> getUsuariosTotales() throws RemoteException{
        ArrayList<String> users = new ArrayList<String>();
        for (int i = 0; i < this.v_replicas.size(); i++) {
            for (int j = 0; j < this.v_replicas.get(i).getUsuarios().size(); j++) 
                users.add(this.v_replicas.get(i).getUsuarios().get(j));
        }
        return users;
    }
    
    public ArrayList<String> getRegistro() throws RemoteException {
        return this.registro;
    }
    
    public void addUsuario(String usuario) throws RemoteException {
        this.usuarios.add(usuario);
    }
    
    public double getTotalLocal() {
        return this.totalLocal;
    }
    
    public void donacion(double cantidad) throws RemoteException {
        this.totalLocal += cantidad;
    }
    
    public void addDonante(String donante) throws RemoteException {
        this.registro.add(donante);
    }

    public ArrayList<I_Donaciones> getReplicas() throws RemoteException{
        return this.v_replicas ;
    }



    public void enlazar(String ubicacion, String nombre) {
        try {
            Registry registry = LocateRegistry.getRegistry(ubicacion);
            I_Donaciones aux = (I_Donaciones) registry.lookup(nombre);

            ArrayList<I_Donaciones> repl = aux.getReplicas();

            if(repl.size() == 0){
                this.v_replicas.add(this);
                this.v_replicas.add(aux);

                aux.addReplica(aux);
                aux.addReplica(this);
                System.out.println("2 servidores enlazados correctamente");
            }
            if(repl.size() >= 2){
                for(int i = 0; i < repl.size(); i++){
                    this.v_replicas.add(repl.get(i));
                    repl.get(i).addReplica(this);
                }
                this.v_replicas.add(this);
                System.out.println((repl.size()+1) +" servidores enlazados correctamente");
            }


        } catch (Exception e) {
            System.err.println("Error al enlazar los servidores");
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println(
                    "Error, uso correcto java Servidor (IP) (0|1|2|3|4)");
            System.exit(0);
        }

        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
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

            I_Donaciones servidor = new Servidor();
            I_Donaciones stub = (I_Donaciones) UnicastRemoteObject.exportObject(servidor, 0);
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind(nombre_objeto_remoto, stub);

            if (Integer.parseInt(args[1]) != 0) {
                ((Servidor) servidor).enlazar(args[0], "Original");
            }

            System.out.println("Servidor "+ args[1] + " se ha iniciado");
        } catch (Exception e) {
            System.err.println("Servidor exception:");
            e.printStackTrace();
        }

    }
}
