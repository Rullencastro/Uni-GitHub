#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;


Semaphore mostr_vacio = 1;
Semaphore ingr_disp[3]={0,0,0};
int num_hebras = 3;



//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

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
//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(  )
{
	int i;
	while(true)
	{	
	i = Producir();
	sem_wait( mostr_vacio );
	cout << "Puesto ingrediente: " << i<<endl;
	sem_signal( ingr_disp[i] );
	}	
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador )
{
   while( true )
   {
	sem_wait( ingr_disp[num_fumador]);
	cout << "Retirado ingrediente: " << num_fumador<<endl;
	sem_signal(mostr_vacio);
	fumar(num_fumador);
   }
}

//----------------------------------------------------------------------

int main()
{
   // declarar hebras y ponerlas en marcha
   // ......

	thread hebra_estanquera(funcion_hebra_estanquero);
	thread hebras[num_hebras];
	for(int i= 0;i < num_hebras;i++){
		hebras[i]= thread (funcion_hebra_fumador,i);
	}

	hebra_estanquera.join();
	for(int i= 0;i < num_hebras;i++){
		hebras[i].join();
	}

}
