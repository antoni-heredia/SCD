#include <iostream>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.hpp"
using namespace std ;
using namespace HM;

mutex
   mtx ;  
template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

const int N_CLIENTES = 3;
class Barberia : public HoareMonitor{
private:
CondVar barbero,
        sala_espera;
bool ocupada;
bool hayClientes;
public:
Barberia();

void cortarPelo(int i);
void siguienteCliente();
void finDeCliente();

};

Barberia::Barberia(){
    barbero = newCondVar();
    sala_espera = newCondVar();
    ocupada = false;
    hayClientes = false;
}

void Barberia::cortarPelo(int i){
    
    if(ocupada)
        sala_espera.wait();
    

    mtx.lock();
    cout << "Se sienta en la silla el cliente: " << i << endl;
    mtx.unlock();
    hayClientes = true;
    
    if(!barbero.empty()){
        mtx.lock();
        cout << "Se despierta el barbero" << endl;
        mtx.unlock();
        barbero.signal();
    }

    
}

void Barberia::siguienteCliente(){
    
    if(!hayClientes){
        mtx.lock();
        cout << "El barbero se duerme" << endl;
        mtx.unlock();
        barbero.wait();        
    }

    ocupada = true;
}

void Barberia::finDeCliente(){
    
    ocupada = false;
    hayClientes = false;
    sala_espera.signal();
}

void esperarFueraBarberia(int i){
    mtx.lock();
    cout << "El cliente: " << i << " espera fuera" << endl;
    mtx.unlock();

    this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
    mtx.lock();
    cout << "El cliente: " << i << " entra" << endl;
    mtx.unlock();

}

void cortarPeloCliente(){
    this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
}

void hebraCliente(MRef<Barberia> barberia, int i){
    while(true){
        barberia->cortarPelo(i);
        esperarFueraBarberia(i);
    }
}

void hebraBarbero(MRef<Barberia> barberia){
    while(true){
        barberia->siguienteCliente();
        cortarPeloCliente();
        barberia->finDeCliente();
    }
}


int main()
{

  MRef<Barberia> barberia = Create<Barberia>();

  thread thread_barbero (hebraBarbero, barberia);
  thread thread_cliente[ 3 ];

  for(int i=0;i < 3; i++)
    thread_cliente[i] = thread( hebraCliente,barberia,i );

   //Hacemos el join con las hebras.
   thread_barbero.join() ;
   for(int j=0;j < 3; j++)
     thread_cliente[j].join();
}
