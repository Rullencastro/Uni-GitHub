import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;


public interface I_Donaciones extends Remote
{
    void registrar(String usuario) throws RemoteException;
    void donar(String usuario,double cantidad) throws RemoteException;
    double getTotal(String usuario) throws RemoteException; 
    ArrayList<String> getUsuarios() throws RemoteException;
    ArrayList<String> getUsuariosTotales() throws RemoteException;
    ArrayList<String> getRegistro() throws RemoteException;
    void addUsuario(String usuario) throws RemoteException;
    double getTotalLocal() throws RemoteException;
    void donacion(double cantidad) throws RemoteException;
    void addDonante(String donante) throws RemoteException;
    void addReplica(I_Donaciones ref)throws RemoteException;
    ArrayList<I_Donaciones> getReplicas() throws RemoteException;
}
