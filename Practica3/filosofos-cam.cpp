// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: filosofos-plantilla.cpp
// Implementación del problema de los filósofos (sin camarero).
// Plantilla para completar.
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
// -----------------------------------------------------------------------------


#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
   num_filosofos = 5 ,
   num_procesos  = 2*num_filosofos+1,
   etiq_coger = 0,
   etiq_soltar = 1,
   etiq_levantarse = 2,
   etiq_sentarse = 3;

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

// ---------------------------------------------------------------------

void funcion_filosofos( int id )
{
  //al tener un tenedor mas tenemos qeu tenerlo en cuenta a la hora de calcular el tenedor que le toca a cada filosofo
  int id_ten_izq = (id+1)              % (num_procesos-1), //id. tenedor izq.
      id_ten_der = (id+num_procesos-2) % (num_procesos-1); //id. tenedor der.

  while ( true )
  {
    MPI_Status estado;
    //El 10 es por el id del camarero
    cout << "El filosofo " << id << "pide sentarse. "  << endl;
    MPI_Send(NULL, 0, MPI_INT, 10, etiq_sentarse, MPI_COMM_WORLD);

    MPI_Recv(NULL, 0, MPI_INT, 10, etiq_sentarse, MPI_COMM_WORLD, &estado);
    cout << "El filosofo " << id << " puede coger tenedores. "  << endl;

    // El filosofo se sienta
    cout <<"Filósofo " <<id << " solicita ten. izq." <<id_ten_izq <<endl;
    MPI_Ssend(NULL, 0, MPI_INT, id_ten_izq, etiq_coger, MPI_COMM_WORLD);

    cout <<"Filósofo " <<id <<" solicita ten. der." <<id_ten_der <<endl;
    MPI_Ssend(NULL, 0, MPI_INT, id_ten_der, etiq_coger, MPI_COMM_WORLD);

    cout <<"Filósofo " <<id <<" comienza a comer" <<endl ;
    sleep_for( milliseconds( aleatorio<10,100>() ) );

    cout <<"Filósofo " <<id <<" suelta ten. izq. " <<id_ten_izq <<endl;
    MPI_Ssend(NULL, 0, MPI_INT, id_ten_izq, etiq_soltar, MPI_COMM_WORLD);

    cout<< "Filósofo " <<id <<" suelta ten. der. " <<id_ten_der <<endl;
    MPI_Ssend(NULL, 0, MPI_INT, id_ten_der, etiq_soltar, MPI_COMM_WORLD);
    
    cout << "El filosofo" << id << " se levanta a pensar." << endl;
    MPI_Ssend(NULL, 0, MPI_INT, 10, etiq_levantarse, MPI_COMM_WORLD );

    cout << "Filosofo " << id << " comienza a pensar" << endl;
    sleep_for( milliseconds( aleatorio<10,100>() ) );
 }
}
// ---------------------------------------------------------------------

void funcion_tenedores( int id )
{
  int valor, id_filosofo ;  // valor recibido, identificador del filósofo
  MPI_Status estado ;       // metadatos de las dos recepciones

  while ( true )
  {


    MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_coger, MPI_COMM_WORLD, &estado);
    id_filosofo = estado.MPI_SOURCE; // Obtiene el rank del filosofo
    cout <<"Ten. " <<id <<" ha sido cogido por filo. " <<id_filosofo <<endl;

    MPI_Recv(&id_filosofo, 1, MPI_INT, id_filosofo, etiq_soltar, MPI_COMM_WORLD, &estado);
    cout <<"Ten. "<< id<< " ha sido liberado por filo. " <<id_filosofo <<endl ;
  }
}

void funcion_camarero(){
  int datos, cant_sentadps=0;
  MPI_Status estado;
  while (true)
  {
      if (cant_sentadps < 4) // El maximo de filosofos comiendo es 4
          MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado); // Puede sentarse o levantarse
      else
          MPI_Probe(MPI_ANY_SOURCE, etiq_levantarse, MPI_COMM_WORLD, &estado);  // Solo puede levantarse


      if (estado.MPI_TAG == etiq_sentarse) // se le deja sentarse
      {
          datos=estado.MPI_SOURCE;
          MPI_Recv( NULL, 0, MPI_INT, datos, etiq_sentarse, MPI_COMM_WORLD,&estado);
          cant_sentadps++;

          MPI_Send( NULL, 0, MPI_INT, datos, etiq_sentarse, MPI_COMM_WORLD);
          cout << "Filosofo " << datos << " se sienta. Hay " << cant_sentadps << " filosofos sentados. " << endl;

      }
      if (estado.MPI_TAG == etiq_levantarse) // Se levanta
      {
          datos=estado.MPI_SOURCE;
          MPI_Recv( NULL, 0, MPI_INT, datos, etiq_levantarse, MPI_COMM_WORLD,&estado);
          cant_sentadps--;
          cout << "Filosofo " << datos << " se levanta. Hay " << cant_sentadps << " filosofos sentados.  " << endl;

      }
  }
}
// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );


   if ( num_procesos == num_procesos_actual )
   {  
    //Ejecuta la funcion del camarero
    if(id_propio == 10)
      funcion_camarero();
      // ejecutar la función correspondiente a 'id_propio'
    else if ( id_propio % 2 == 0 )          // si es par
        funcion_filosofos( id_propio ); //   es un filósofo
    else                               // si es impar
        funcion_tenedores( id_propio ); //   es un tenedor
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------
