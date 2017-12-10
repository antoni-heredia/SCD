#include <iostream>
#include <cassert>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.hpp"

using namespace std ;
using namespace HM ;

const int CANT_INGRE = 3;
mutex mtx ; 
class Estanco : public HoareMonitor{

private:
  
  bool hayIngrediente[CANT_INGRE];
  bool ingrediente;
  CondVar estanquero;
  CondVar fumador[CANT_INGRE];
public:
  Estanco();
  void obtenerIngrediente(int i);
  void ponerIngrediente(int i);
  void esperarRecogidaIngrediente();
          
};

Estanco::Estanco(){

  ingrediente = false;
  estanquero = newCondVar();
  //Por defecto no hay ningun ingrediente
  for(int i = 0; i<CANT_INGRE; i++){
    fumador[i] = newCondVar();
    hayIngrediente[i] = false;
  }

}

void Estanco::obtenerIngrediente(int i ){
  //Si no hay ingrediente esperamos
  if(!hayIngrediente[i])
    fumador[i].wait();

  estanquero.signal();
  hayIngrediente[i] = false;
  ingrediente = false;
}

void Estanco::ponerIngrediente(int i){
  ingrediente= true;
  hayIngrediente[i] = true;
  cout << "El estanquero a producido el ingrediente: " << i << endl;
  fumador[i].signal();
}

void Estanco::esperarRecogidaIngrediente(){
  if(ingrediente)
    estanquero.wait();
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

void funcion_hebra_estanquero( MRef<Estanco> estanco)
{
  int i;
  while(true){
    i = Producir();
    estanco->ponerIngrediente(i);
    estanco->esperarRecogidaIngrediente();
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
    mtx.unlock();

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( MRef<Estanco> estanco, int num_fumador )
{
   while( true )
   {

    estanco->obtenerIngrediente(num_fumador);
    mtx.lock();
    cout << "retirado ingr: " << num_fumador << endl;
    mtx.unlock();

    fumar(num_fumador);
   }
}

//----------------------------------------------------------------------

int main()
{

  MRef<Estanco> monitor = Create<Estanco>();

  thread thread_estan (funcion_hebra_estanquero, monitor);
  thread thread_fumadores [ 3 ];

  for(int i=0;i < CANT_INGRE; i++)
    thread_fumadores[i] = thread( funcion_hebra_fumador,monitor,i );

   //Hacemos el join con las hebras.
   thread_estan.join() ;
   for(int j=0;j < CANT_INGRE; j++)
     thread_fumadores[j].join();
}
