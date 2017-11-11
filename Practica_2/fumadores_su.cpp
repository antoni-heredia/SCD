#include <iostream>
#include <cassert>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.hpp"

using namespace std ;
using namespace HM ;

class MFumadoresSU : public HoareMonitor
{



}
template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

int Producir(){
  //Generamos un retardo
  this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
  //Devolvemos un aleatorio entre 0 y 2.
  return aleatorio<0,2>();
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero
Semaphore mostr_vacio = 1;
Semaphore ingr_disp[3] = {0,0,0};
void funcion_hebra_estanquero(  )
{
  int i;
  while(true){
    mostr_vacio.sem_wait();
    i = Producir();
    cout << "El estanquero a producido el ingrediente: " << i << endl;
    ingr_disp[i].sem_signal();
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
      sem_wait(ingr_disp[num_fumador]);
      cout << "retirado ingr: " << num_fumador << endl;
      sem_signal(mostr_vacio);
      fumar(num_fumador);
   }
}

//----------------------------------------------------------------------

int main()
{
  thread thread_estan (funcion_hebra_estanquero);

  //Vector con los fumadores
  thread thread_fumadores [ 3 ];
  for(int i=0;i < 3; i++)
    thread_fumadores[i] = thread( funcion_hebra_fumador,i );

   //Hacemos el join con las hebras.
   thread_estan.join() ;
   for(int j=0;j < 3; j++)
     thread_fumadores[j].join();
}
