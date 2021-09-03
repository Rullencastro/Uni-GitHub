#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include "HoareMonitor.h"

using namespace std ;
using namespace HM;

constexpr int
   num_items  = 40 ,     // número de items a producir/consumir
    num_hebras_productoras = 8,
    num_hebras_consumidoras = 8;
mutex
   mtx ;                 // mutex de escritura en pantalla
unsigned
   cont_prod[num_items], // contadores de verificación: producidos
   cont_cons[num_items], // contadores de verificación: consumidos
   producidos[num_hebras_productoras];

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

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

int producir_dato(int i)
{
   int p = num_items/num_hebras_productoras;
   static int contador = 0 ;
   int a = i * p + producidos[i];  
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   mtx.lock();
   cout << "producido(hebra " << i <<" ): " << a << endl << flush ;
   mtx.unlock();
   producidos[i]++;
   cont_prod[contador] ++ ;
   return a ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   if ( num_items <= dato )
   {
      cout << " dato === " << dato << ", num_items == " << num_items << endl ;
      assert( dato < num_items );
   }
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   mtx.lock();
   cout << "                  consumido: " << dato << endl ;
   mtx.unlock();
}
//----------------------------------------------------------------------

void ini_contadores()
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  cont_prod[i] = 0 ;
      cont_cons[i] = 0 ;
   }
}

//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." << flush ;

   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      if ( cont_prod[i] != 1 )
      {
         cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {
         cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

// *****************************************************************************
// clase para monitor buffer, version LIFO, semántica SC, un prod. y un cons.

class ProdCons2SU : public HoareMonitor
{
 private:
 static const int           // constantes:
   num_celdas_total = 10;   //  núm. de entradas del buffer
 int                        // variables permanentes
   buffer[num_celdas_total],//  buffer de tamaño fijo, con los datos
   primera_libre ;          //  indice de celda de la próxima inserción
 CondVar 
    ocupadas,
    libres;                 //  cola de hebras esperando

 public:                    // constructor y métodos públicos
   ProdCons2SU(  ) ;           // constructor
   int  leer();                // extraer un valor (sentencia L) (consumidor)
   void escribir( int valor ); // insertar un valor (sentencia E) (productor)
} ;
// -----------------------------------------------------------------------------

ProdCons2SU::ProdCons2SU()
{
   primera_libre = 0 ;
   for(int i = 0;i < num_hebras_productoras;i++)
    producidos[i] = 0;
    ocupadas = newCondVar();
    libres = newCondVar();
}
// -----------------------------------------------------------------------------
// función llamada por el consumidor para extraer un dato

int ProdCons2SU::leer(  )
{


   // esperar bloqueado hasta que 0 < num_celdas_ocupadas
   while( primera_libre == 0 )
      ocupadas.wait();

   // hacer la operación de lectura, actualizando estado del monitor
   assert( 0 < primera_libre  );
   primera_libre-- ;
   const int valor = buffer[primera_libre] ;


   // señalar al productor que hay un hueco libre, por si está esperando
   libres.signal();

   // devolver valor
   return valor ;
}
// -----------------------------------------------------------------------------

void ProdCons2SU::escribir( int valor )
{

   // esperar bloqueado hasta que num_celdas_ocupadas < num_celdas_total
   while( primera_libre == num_celdas_total )
      libres.wait();

   //cout << "escribir: ocup == " << num_celdas_ocupadas << ", total == " << num_celdas_total << endl ;
   assert( primera_libre < num_celdas_total );

   // hacer la operación de inserción, actualizando estado del monitor
   buffer[primera_libre] = valor ;
   primera_libre++ ;

   // señalar al consumidor que ya hay una celda ocupada (por si esta esperando)
   ocupadas.signal();
}
// *****************************************************************************
// funciones de hebras

void funcion_hebra_productora( MRef<ProdCons2SU> monitor ,int i)
{
   int p = num_items/num_hebras_productoras;
   for(unsigned j = i*p; j < i*p+p; j++)
   {
        int valor = producir_dato(i);
        monitor->escribir( valor );
   }
}
// -----------------------------------------------------------------------------

void funcion_hebra_consumidora( MRef<ProdCons2SU> monitor)
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      int valor = monitor->leer();
      consumir_dato( valor ) ;
   }
}
// -----------------------------------------------------------------------------

int main()
{
   cout << "-------------------------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (8 prod/cons, Monitor SC, buffer LIFO). " << endl
        << "-------------------------------------------------------------------------------" << endl
        << flush ;

   MRef<ProdCons2SU> monitor = Create<ProdCons2SU>();

   thread hebras_productoras[num_hebras_productoras],
          hebras_consumidoras[num_hebras_consumidoras];

    for(int i = 0;i < num_hebras_productoras; i++){
        hebras_productoras[i] = thread (funcion_hebra_productora, monitor,i);
    }
    for(int i = 0;i < num_hebras_consumidoras; i++){
        hebras_consumidoras[i] = thread(funcion_hebra_consumidora, monitor);
    }
    for(int i = 0;i < num_hebras_productoras; i++){
        hebras_productoras[i].join() ;
    }
    for(int i = 0;i < num_hebras_consumidoras; i++){
        hebras_consumidoras[i].join() ;
    }

   // comprobar que cada item se ha producido y consumido exactamente una vez
   test_contadores() ;
}