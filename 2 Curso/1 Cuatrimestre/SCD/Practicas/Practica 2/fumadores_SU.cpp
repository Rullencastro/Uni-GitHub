#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
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

//----------------------------------------------------------------------
int Producir()
{
	int a = aleatorio<0,2>();
	chrono::milliseconds duracion_producir( aleatorio<20,200>() );
	this_thread::sleep_for( duracion_producir );
	return a;
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar
	mtx.lock();
    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;
	mtx.unlock();
   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar
	mtx.lock();
    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;
	mtx.unlock();
}

//----------------------------------------------------------------------
//Monitor de SU
class Estanco : public HoareMonitor{
	private:
		int 
			ing_actual;
		bool
			libre;
		CondVar
			colaF1,
			colaF2,
			colaF3,
			colaE;
	public:
		Estanco();
		void obtenerIngrediente(int i);
		void ponerIngrediente(int i);
		void esperarRecogidaIngrediente();
};
//----------------------------------------------------------------------

Estanco::Estanco(){
	colaF1 = newCondVar();
	colaF2 = newCondVar();
	colaF3 = newCondVar();
	colaE = newCondVar();
	libre = true;
	ing_actual = 4;
}

void Estanco::ponerIngrediente(int i)
{
	ing_actual = i;
	mtx.lock();
	cout << "Puesto ingrediente: "<< ing_actual<<endl;
	mtx.unlock();
	libre = false;
	if(ing_actual == 0){
		colaF1.signal();
	}
	else if(ing_actual == 1){
		colaF2.signal();
	}
	else{
		colaF3.signal();
	}
}

void Estanco::obtenerIngrediente(int i)
{
 	if(i != ing_actual && i == 0){
 		colaF1.wait();
 	}
	else if(i != ing_actual && i == 1){
 		colaF2.wait();
 	}
	else if(i != ing_actual && i == 2){
 		colaF3.wait();
 	}
 	libre = true;
	 ing_actual = 4;
 	colaE.signal();
 	mtx.lock();
 	cout << "Retirado ingrediente: " << i << endl;
 	mtx.unlock();
}

void Estanco::esperarRecogidaIngrediente()
{
	if(libre == false)
		colaE.wait();
	else
		colaE.signal();
}
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(MRef<Estanco> monitor)
{
	int ing = 0;
	while(true)
	{	
	ing = Producir();
	cout << "Producido: " << ing <<endl;
	monitor->ponerIngrediente(ing);
	monitor->esperarRecogidaIngrediente();
	}	
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( MRef<Estanco> monitor, int num_fumador )
{
   while( true )
   {
	monitor->obtenerIngrediente(num_fumador);
	fumar(num_fumador);
   }
}

int main()
{
   // declarar hebras y ponerlas en marcha
   // ......

	MRef<Estanco> monitor = Create<Estanco>();

	thread hebra_estanquera(funcion_hebra_estanquero,monitor);
	thread hebras[3];
	for(int i= 0;i < 3;i++){
		hebras[i]= thread (funcion_hebra_fumador,monitor,i);
	}

	hebra_estanquera.join();
	for(int i= 0;i < 3;i++){
		hebras[i].join();
	}

}
