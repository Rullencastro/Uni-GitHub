#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <condition_variable>
#include "HoareMonitor.h"

using namespace std ;
using namespace HM ;

mutex
	mtx;


template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

int Beber_estudiar(int alumno)
{
	chrono::milliseconds beber_estudiar( aleatorio<40,400>() );
	mtx.lock();
	cout << alumno << " Esta bebiendo/estudiando " << endl;
	mtx.unlock();
    	this_thread::sleep_for( beber_estudiar ); 
}

class Bareto : public HoareMonitor{
	private:
	int
		cervezas = 6;
	CondVar
		camarero,
		borrachuzo;

	public:
		Bareto();
		void pedir(int decision);
		void servir();
};

Bareto::Bareto()
{
	camarero = newCondVar();
	borrachuzo = newCondVar();
}

void Bareto::pedir(int decision)
{
	if(decision == 0){			//Borrachuzo
		if(cervezas == 0){
			borrachuzo.wait();
		}
		mtx.lock();
		cout << "Borracho se pone a beber" << endl;
		mtx.unlock();
		cervezas--;
	}
	if(decision == 1){			//Responsable
		mtx.lock();
		cout << "Responsable quita cervezas de la barra" << endl;
		mtx.unlock();
		if(cervezas >= 2){
			cervezas = cervezas - 2;
		}
		else{
			cervezas = 0;
		}
	}
	if(cervezas == 0){
		mtx.lock();
		cout << "No hay cervezas, se llama al camarero" << endl;
		mtx.unlock();
		camarero.signal();
	}
}

void Bareto::servir()
{
	if(cervezas > 0){		
		mtx.lock();
		cout << "Camarero duerme" << endl;
		mtx.unlock();
		camarero.wait();
	}
	mtx.lock();
	cout << "Se repone la barra" << endl;
	mtx.unlock();
	cervezas = 6;
	borrachuzo.signal();
}


void hebra_alumno_SCD(MRef<Bareto> monitor, int alumno)
{
	int actitud;
	while(true)
	{
		actitud = aleatorio<0,1>();
		monitor->pedir(actitud);
		Beber_estudiar(alumno);
	}
}

void hebra_camarero(MRef<Bareto> monitor)
{
	while(true)
	{
		monitor->servir();
	}
}

int main(){
	
	MRef<Bareto> monitor = Create<Bareto>();
		
	thread hebra_camarero(hebra_camarero,monitor);
	thread hebra1 (hebra_alumno_SCD, monitor,0);
	thread hebra2 (hebra_alumno_SCD, monitor,1);
	thread hebra3 (hebra_alumno_SCD, monitor,2);	


	hebra_camarero.join();
	hebra1.join();
	hebra2.join();
	hebra3.join();
}
















	
			














