// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: prodcons2.cpp
// Implementación del problema del productor-consumidor con
// un proceso intermedio que gestiona un buffer finito y recibe peticiones
// en orden arbitrario
// (versión con un único productor y un único consumidor)
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
// -----------------------------------------------------------------------------

#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
  np=10,
  nc=2,
  etiq_productor          = 0 , // identificador del proceso productor
  etiq_consumidor         = 2 , // identificador del proceso consumidor
  num_items             = 20,
  tam_vector            = 10;

const int
  num_procesos_esperado = np+nc+1 , // número total de procesos esperado
  k_pro = num_items/np,
  k_consumir = num_items/nc,
  id_buffer             = np; // identificador del proceso buffer



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
// ptoducir produce los numeros en secuencia (1,2,3,....)
// y lleva espera aleatorio
int producir(int num_orden, int i)
{
   cout << "Productor ha producido valor " << num_orden*k_pro+i << endl << flush;
   return num_orden*k_pro+i ;
}
// ---------------------------------------------------------------------

void funcion_productor(int num_orden)
{ 
   for ( unsigned int i= 0 ; i < k_pro ; i++ )
   {
      // producir valor
      int valor_prod = producir(num_orden, i);
      // enviar valor
      cout << "Productor va a enviar valor " << valor_prod << endl << flush;
      MPI_Ssend( &valor_prod, 1, MPI_INT, id_buffer, etiq_productor, MPI_COMM_WORLD );
   }
}
// ---------------------------------------------------------------------

void consumir( int valor_cons )
{
   // espera bloqueada
   sleep_for( milliseconds( aleatorio<110,200>()) );
   cout << "Consumidor ha consumido valor " << valor_cons << endl << flush ;
}
// ---------------------------------------------------------------------

void funcion_consumidor(int num_orden)
{
   int         peticion,
               valor_rec = 1 ;
   MPI_Status  estado ;

   for( unsigned int i=k_consumir*num_orden ; i < k_consumir*num_orden+k_consumir; i++ )
   {

      MPI_Ssend( &peticion,  1, MPI_INT, id_buffer, etiq_consumidor, MPI_COMM_WORLD);
      MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer, etiq_consumidor, MPI_COMM_WORLD,&estado );
      cout << "Consumidor ha recibido valor " << valor_rec << endl << flush ;
      consumir( valor_rec );
   }
}
// ---------------------------------------------------------------------

void funcion_buffer()
{
   int        buffer[tam_vector],      // buffer con celdas ocupadas y vacías
              valor,                   // valor recibido o enviado
              primera_libre       = 0, // índice de primera celda libre
              primera_ocupada     = 0, // índice de primera celda ocupada
              num_celdas_ocupadas = 0, // número de celdas ocupadas
              tag_emisor_aceptable ;    // identificador de emisor aceptable
   MPI_Status estado ;                 // metadatos del mensaje recibido

   for( unsigned int i=0 ; i < num_items*2 ; i++ )
   {
      // 1. determinar si puede enviar solo prod., solo cons, o todos

      if ( num_celdas_ocupadas == 0 )               // si buffer vacío
         tag_emisor_aceptable = etiq_productor ;       // $~~~$ solo prod.
      else if ( num_celdas_ocupadas == tam_vector ) // si buffer lleno
         tag_emisor_aceptable = etiq_consumidor ;      // $~~~$ solo cons.
      else                                          // si no vacío ni lleno
         tag_emisor_aceptable = MPI_ANY_TAG ;     // $~~~$ cualquiera

      // 2. recibir un mensaje del emisor o emisores aceptables
      MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, tag_emisor_aceptable, MPI_COMM_WORLD, &estado );

      // 3. procesar el mensaje recibido

      switch( estado.MPI_TAG ) // leer emisor del mensaje en metadatos
      {
         case etiq_productor: // si ha sido el productor: insertar en buffer
            buffer[primera_libre] = valor ;
            primera_libre = (primera_libre+1) % tam_vector ;
            num_celdas_ocupadas++ ;
            cout << "Buffer ha recibido valor " << valor << endl ;
            break;

         case etiq_consumidor: // si ha sido el consumidor: extraer y enviarle
            valor = buffer[primera_ocupada] ;
            primera_ocupada = (primera_ocupada+1) % tam_vector ;
            num_celdas_ocupadas-- ;
            cout << "Buffer va a enviar valor " << valor << endl ;
            MPI_Ssend( &valor, 1, MPI_INT, estado.MPI_SOURCE, etiq_consumidor, MPI_COMM_WORLD);
            break;
      }
   }
}

// ---------------------------------------------------------------------

int main( int argc, char *argv[] )
{
   int id_propio, num_procesos_actual;

   // inicializar MPI, leer identif. de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if ( num_procesos_esperado == num_procesos_actual )
   {
    MPI_Status estado ;
    int id_vecino;
    int valor_pro;
    if ( id_propio >= 0 && id_propio <= np-1 )  // si mi ident. es el del productor
      if(id_propio ==  0 ){
        valor_pro = 0;
        funcion_productor(valor_pro);            //    ejecutar función del productor
        valor_pro++;
        id_vecino = id_propio + 1 ;
        cout << "El productor " << id_propio << " envia el valor " << valor_pro << " al productor " << id_vecino << flush;
        MPI_Ssend( &valor_pro,  1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD );

      }else if(id_propio > 0 && id_propio < np-1){

        id_vecino = id_propio-1 ;
        MPI_Recv ( &valor_pro, 1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD, &estado );
        funcion_productor(valor_pro);            //    ejecutar función del productor

        id_vecino = id_propio+1;
        valor_pro++;
        cout << "El productor " << id_propio << " envia el valor " << valor_pro << " al productor " << id_vecino << flush;
        MPI_Ssend( &valor_pro,  1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD );
      }else{
        id_vecino = id_propio-1 ;
        MPI_Recv ( &valor_pro, 1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD, &estado );
        funcion_productor(valor_pro);            //    ejecutar función del productor
        
      }
    else if ( id_propio == np )// si mi ident. es el del buffer
      funcion_buffer();               //    ejecutar función buffer
    else
      if(id_propio ==  np+1 ){
        valor_pro = 0;
        funcion_consumidor(valor_pro);            //    ejecutar función del productor
        valor_pro++;
        id_vecino = id_propio + 1 ;
        cout << "El productor " << id_propio << " envia el valor " << valor_pro << " al productor " << id_vecino << flush;
        MPI_Ssend( &valor_pro,  1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD );

      }else if(id_propio > np+1 && id_propio < np+nc){

        id_vecino = id_propio-1 ;
        MPI_Recv ( &valor_pro, 1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD, &estado );
        funcion_consumidor(valor_pro);            //    ejecutar función del productor

        id_vecino = id_propio+1;
        valor_pro++;
        cout << "El productor " << id_propio << " envia el valor " << valor_pro << " al productor " << id_vecino << flush;
        MPI_Ssend( &valor_pro,  1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD );
      }else{
        id_vecino = id_propio-1 ;
        MPI_Recv ( &valor_pro, 1, MPI_INT, id_vecino, 10, MPI_COMM_WORLD, &estado );
        funcion_consumidor(valor_pro);            //    ejecutar función del productor
      }
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos_esperado << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   // al terminar el proceso, finalizar MPI
   MPI_Finalize( );
   return 0;
}
