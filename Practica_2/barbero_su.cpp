#include <iostream>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.hpp"
using namespace std ;
using namespace HM;

  
template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

const int N_CLIENTES = 5;
class Barberia : public HoareMonitor{
private:
CondVar barbero,
        sala_espera,
        silla;
int clienteActual;
public:
Barberia();

void cortarPelo(int i);
void siguienteCliente();
void finDeCliente();

};

Barberia::Barberia(){
    barbero = newCondVar();
    sala_espera = newCondVar();
    silla = newCondVar();
    
}

void Barberia::cortarPelo(int i){
    while(!silla.empty())
        sala_espera.wait();

    
    if(!barbero.empty()){
        cout << "Cliente: " << i << " despierta al barbero" << endl;
    }
    cout << "Cliente: " << i << " se sienta en la silla" << endl;
    barbero.signal();
    
    silla.wait();
}

void Barberia::siguienteCliente(){
    
    if(silla.empty() && sala_espera.empty()){
        cout << "El barbero duerme" << endl;

        barbero.wait();

    }

    sala_espera.signal();

}

void Barberia::finDeCliente(){
        silla.signal();
}

void esperarFueraBarberia(int i){
    cout << "El cliente: " << i << " espera fuera" << endl;

    this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

    cout << "El cliente: " << i << " entra" << endl;

}

void cortarPeloCliente(){
    cout << "Se corta el pelo al cliente" << endl;
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
  thread thread_cliente[ N_CLIENTES ];

  for(int i=0;i < N_CLIENTES; i++)
    thread_cliente[i] = thread( hebraCliente,barberia,i );

   //Hacemos el join con las hebras.
   thread_barbero.join() ;
   for(int j=0;j < N_CLIENTES; j++)
     thread_cliente[j].join();
}
