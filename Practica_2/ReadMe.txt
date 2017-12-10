 ----------------------Fumadores-----------------------
Se compila con g++ -std=c++11 -pthread -Wfatal-errors -I. -o fumadores_su.out fumadores_su.cpp  HoareMonitor.cpp

Una es "ingrediente" que la utilizo para saber si hay algun ingrediente puesto en el monitor.
  Tambien utilizo un array de boleanos que es "hayIngrediente" que tendra en cada posicion si hay un ingrediente de ese tipo o no.
Mis colas de condicion son:
  -"estanquero" el estanquero hara un wait si en el mostrador ya hay un ingrediente puesto y signal una vez que un fumador coge el ingrediente
    +wait -> ingrediente == true
  -"fumador[CANT_INGRE]" que es un array de fumadores. En cada posicion hay una cola para  un fumador. Cada fumador hara wait cuando no hay  del ingrediente 
  que el necesita y hara el signal una vez que el estanquero a producido de su ingrediente
    +wait -> hayIngrediente[i] == false

Listado de salida que se pide:

El estanquero a producido el ingrediente: 1
retirado ingr: 1
Fumador 1  : empieza a fumar (162 milisegundos)
El estanquero a producido el ingrediente: 0
retirado ingr: 0
Fumador 0  : empieza a fumar (119 milisegundos)
El estanquero a producido el ingrediente: 0
Fumador 1  : termina de fumar, comienza espera de ingrediente.
Fumador 0  : termina de fumar, comienza espera de ingrediente.
retirado ingr: 0
Fumador 0  : empieza a fumar (39 milisegundos)
Fumador 0  : termina de fumar, comienza espera de ingrediente.
El estanquero a producido el ingrediente: 0
retirado ingr: 0
Fumador 0  : empieza a fumar (172 milisegundos)
El estanquero a producido el ingrediente: 1
retirado ingr: 1
Fumador 1  : empieza a fumar (71 milisegundos)
El estanquero a producido el ingrediente: 1
Fumador 1  : termina de fumar, comienza espera de ingrediente.
retirado ingr: 1
Fumador 1  : empieza a fumar (106 milisegundos)
Fumador 0  : termina de fumar, comienza espera de ingrediente.
El estanquero a producido el ingrediente: 1
Fumador 1  : termina de fumar, comienza espera de ingrediente.
retirado ingr: 1
Fumador 1  : empieza a fumar (60 milisegundos)
Fumador 1  : termina de fumar, comienza espera de ingrediente.
El estanquero a producido el ingrediente: 2
retirado ingr: 2
Fumador 2  : empieza a fumar (45 milisegundos)

 ----------------------Barbero-----------------------
Se compila con g++ -std=c++11 -pthread -Wfatal-errors -I. -o barbero_su.out barbero_su.cpp  HoareMonitor.cpp


 En el problema de los barberos tengo 3 variables de condicion.
 Una que representa la silla del barbero, otra la sala  de espera y otra que representa el barbero

 Condvar barbero;

 El barbero se utiliza en cortarPelo(int i) y en siguienteCliente().
 Este realiza el wait si la silla esta vacia y no hay nadie en la sala de espera, es decir -> (silla.empty() && sala_espera.empty()) == true (linea 58)
 El signal lo realiza en cortarPelo(int i) una vez que el un cliente a salido de la sala de espera (linea 48)

Condvar sala_espera;

La sala_espera se utiliza en siguienteCliente() y en cortarPelo(int i).
Esta realiza el wait si la silla esta ocupada, es decir ->  !silla.empty() == true (linea 41)
Esta realiza el signal en siguienteCliente() una vez que el barbero se ha desperatado en caso de que estuviera durmiendo (linea 62)

Condvar silla;

La silla se utiliza en finDeCliente() y en cortarPelo(int i)
Esta realiza el wait  cuando el barbero esta cortando el pelo.
El signal lo realiza cuando el barbero dice que a acabado en finDeCliente()