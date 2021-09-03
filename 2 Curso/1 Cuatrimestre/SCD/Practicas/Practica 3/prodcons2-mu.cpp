//Ejercicio productor consumidor MPI.

#include <mpi.h>
#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo


using namespace std;

const int    
    id_buffer = 4,             // identificador del proceso buffer        
    num_procesos_esperado = 10, // número total de procesos esperado
    num_items = 20,
          etiq_consumidor = 1,  //etiqueta del consumidor
          etiq_productor = 2;   //etiqueta del productor
int tam_vector = 10;            //tamaño del vector
int np = 4, nc = 5;             
int k = num_items / np;         


template< int min, int max > int aleatorio()            //funcion para producir numero aleatorio
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}


int producir_dato(int num_orden_productor)          
{
    static int contador=0;
    int devolver=0;
    this_thread::sleep_for(chrono::milliseconds(aleatorio<10, 50>()));     
    contador++;
    devolver = (num_orden_productor*k)+contador;        
    cout << "Productor de orden " << num_orden_productor << " produce: " << devolver << endl;
    return devolver;
}

void consumir_dato(unsigned dato)
{
    this_thread::sleep_for(chrono::milliseconds(aleatorio<20, 100>()));

    cout << "                  consumido: " << dato << endl;
}

void funcion_productor(int num_orden)
{
    for (int i = (num_orden*k); i <(num_orden*k)+k ; i++)           //Para que cada productor produzca 5 datos.
    {
        int valor_prod = producir_dato(num_orden); 
        cout << "-----------Productor " << num_orden << " va a enviar valor " << valor_prod << endl;
        MPI_Ssend(&valor_prod, 1, MPI_INT, id_buffer, etiq_productor, MPI_COMM_WORLD);          //El productor envia el dato al buffer.
    }
}

void funcion_consumidor(int num_orden)
{
    int peticion, valor_rec = 1;
    MPI_Status estado;
    for (int i = 0; i < num_items/nc; i++)
    {
        MPI_Ssend(&peticion, 1, MPI_INT, id_buffer, etiq_consumidor, MPI_COMM_WORLD);   //Manda una peticion al buffer para coger un dato
        MPI_Recv(&valor_rec, 1, MPI_INT, id_buffer, 0, MPI_COMM_WORLD, &estado);        //Recibe el dato del buffer.
        cout << "Consumidor " << num_orden << " ha recibido valor " << valor_rec << endl;
        consumir_dato(valor_rec); // consumir (espera bloqueado tiempo aleat.)
    }
}

void funcion_buffer()
{
    int buffer[tam_vector],      //buffer con celdas ocupadas y vacías
        valor=1,                   //valor recibido o enviado
        primera_libre = 0,       //índice de primera celda libre
        primera_ocupada = 0,     //índice de primera celda ocupada
        num_celdas_ocupadas = 0, //número de celdas ocupadas
        etiq_aceptable;          //etiqueta de emisor aceptable
    MPI_Status estado;           //metadatos del mensaje recibido

    for (unsigned int i = 0; i < num_items * 2; i++)  //Lo haga 40 veces ,ya que son 20 para productor y otras 20 para consumidor
    {
        if (num_celdas_ocupadas == 0)             //igualo en vez de los id las etiquetas      
            etiq_aceptable = etiq_productor;
        else if (num_celdas_ocupadas == tam_vector)
        {
            etiq_aceptable = etiq_consumidor;
        }
        else
        {
            etiq_aceptable = MPI_ANY_TAG;
        }

        MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_aceptable, MPI_COMM_WORLD, &estado); // indicando con "ANY_SOURCE" que se puede recibir un mensaje de cualquier proceso en el comunicador y utilizando la etiqueta_aceptable comot tag

        switch (estado.MPI_TAG) //el switch se realiza segun el tag del proceso que se ha recibido en el proceso anterior
        {
        case etiq_productor:
            buffer[primera_libre] = valor;
            primera_libre = (primera_libre + 1) % tam_vector;
            num_celdas_ocupadas++;
            cout << "Buffer ha recibido valor " << valor << endl;
            break;

        case etiq_consumidor:

            valor = buffer[primera_ocupada];
            primera_ocupada = (primera_ocupada + 1) % tam_vector;
            num_celdas_ocupadas--;
            cout << "Buffer va a enviar valor " << valor << endl;
            MPI_Ssend(&valor, 1, MPI_INT, estado.MPI_SOURCE, 0, MPI_COMM_WORLD); //Envia al proceso correpondiente el valor .
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int id_propio, num_procesos_actual;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos_actual);

    if(num_procesos_esperado==num_procesos_actual)      //si se cumple 
    {
        if(id_propio<np){                               //si el id propio es menor que el numero de procesos usamos funcion_productor
            funcion_productor(id_propio);           
        }
        else if(id_propio==np)                           //si el id propio es igual que el numero de procesos usamos funcion_buffer  
        {
            funcion_buffer();
        }
        else
        {
            funcion_consumidor(id_propio-np);            //si no se cumple ninguna de las dos condicones usamos la funcion_consumidor
        }
    
    }

    else if(id_propio==0)                           //En caso de que no se cumpla la primera condicion detenemos ejecución 
    {
        cerr << "el número de procesos esperados es:    " << num_procesos_esperado << endl
         << "el número de procesos en ejecución es: " << num_procesos_actual << endl
         << "(programa abortado)" << endl ;
    }

     MPI_Finalize(); // terminar MPI: debe llamarse siempre por cada proceso.
     return 0;        // terminar proceso
}
