#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.h"

using namespace std ;
using namespace HM ;

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

void cortarPeloACliente()
{
    chrono::milliseconds cortar( aleatorio<20,200>() );
    this_thread::sleep_for( cortar ); 
}

void esperarFueraBarberia(int i)
{
    chrono::milliseconds espera( aleatorio<20,200>() );
    cout << "Cliente " << i << " espera fuera(" << espera.count() << " milisegundos)" << endl;
}

class Barberia : public HoareMonitor
{
    private:
        CondVar
            sala_espera,
            silla_pelar,
            barbero;
    public:
        Barberia();
        void siguienteCliente();
        void finCliente();
        void cortarPelo(int i);
};

//Constructor
Barberia::Barberia()
{
    sala_espera = newCondVar();
    silla_pelar = newCondVar();
    barbero = newCondVar();
}

//Funcíon que manda a la sala de espera a los clientes cuando la silla esta ocupada, y que activa al barbero
void Barberia::cortarPelo(int i)
{
    cout << "Entra en la barberia " << i  << endl;
    if(!silla_pelar.empty()){                                           //Compruebo si hay alguien pelandose
        cout << "Cliente " << i << " se va a la sala de espera" << endl;
        sala_espera.wait();                                             //Le digo que espere en la sala de espera
    }
    barbero.signal();                                                   //Le digo al barbero que estoy en la silla de pelar
    cout << "Pela al cliente " << i << "." << endl;
    silla_pelar.wait();                                                 //Espero que me pele
}

//Funcion que llama al siguiente cliente de la sala de espera, en caso de no haber, se duerme el barbero
void Barberia::siguienteCliente()
{
    if(silla_pelar.empty()){                                            //Si no hay nadie pelandose
        if(sala_espera.empty()){                                        //Si no hay nadie en la sala de espera
            cout << "Barbero duerme." << endl;
            barbero.wait();                                             //El barbero se duerme, hasta que haya alguien en la silla de pelar
        }
        else{                                                           //En caso de que haya alguien en la sala de espera
            cout << "Barbero llama a cliente de la sala de espera." << endl;
            sala_espera.signal();                                       //Le digo que entre el siguiente de la sala de espera
        }
    }
}

//Función que avisa de que el barbero ha terminado de pelar
void Barberia::finCliente()
{
    cout << "Barbero termina de pelar." << endl;
    silla_pelar.signal();
}

void funcion_cliente(MRef<Barberia> monitor, int i)
{
    while(true){
        monitor->cortarPelo(i);
        esperarFueraBarberia(i);
    }
}

void funcion_barbero(MRef<Barberia> monitor)
{
    while(true){
        monitor->siguienteCliente();
        cortarPeloACliente();
        monitor->finCliente();
    }
}

int main()
{
    MRef<Barberia> monitor = Create<Barberia>();

    thread hebra_barbero(funcion_barbero, monitor);
    thread hebra_cliente[10];
    for(int i = 0; i < 10; i++){
        hebra_cliente[i] = thread(funcion_cliente, monitor, i);
    }

    hebra_barbero.join();
    for(int j = 0; j < 10; j++){
        hebra_cliente[j].join();
    }
}