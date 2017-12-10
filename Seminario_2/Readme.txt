Actividad Transparencia 25 (Pag 33)

    + En Señalar y continuar, como la hebra que entra la ultima, no realiza ningun tipo de wait, es la que tiene la exclusion mutua hasta que termina su ejecución. Y por lo tanto la primera en salir.
    
    - Como todas las hebras se ponen a la espera, antes de la terminacion y notify_once despierta de forma aleatoria a una no sabemos cual va a salir.

    + Por que el  constructor como solo se ejecuta por la hebra main, no hay problema de que ninguna otra hebra modifique sus datos, no obstante ademas como solo se realiza una operacion de asignacion no habria problemas de resultados inconsistentes.
    
    - Si uso notify_all se puede apreciar como las hebras salen de forma ordenada, aunque segun el manual de c++ no deberia, imagino que es porque el procesaodor al llevar mas tiempo
    en espera decide sacar unas antes que otras.

Actividad Transparencia 36 (Pag 46)
 Se compila con g++ -std=c++11 -pthread -Wfatal-errors -I. -o prodcons1_sc_FIFO.out prodcons1_sc_FIFO.cpp  

    +Los principales cambios que hay que realizar es añadir dos variables mas a el codigo, uno es la primera_ocuapda y otro es n. 
        n nos sirve para ver cuantos elementos hay en un momento dado en el buffer y poder asi  ver cuando se tiene que bloquear una operacion de lectura o de escritura.
        n aumenta cada vez que se escribe un elemento en el buffer y decrementa cada vez que se lee.
        primera_ocuapda  sera una variable que aumenta cada vez que se lee un elemento del buffer. La lectura del buffer sera primera_ocuapda%num_celdas_total.
        primera_libre sera la variable que nos indica donde tenemos que escribir en el buffer en un momento dado. Esta variable aumenta cada vez que se realiza una escritura en el buffer.
        la escritura en el buffer se realiza en la posicion primera_libre%num_celdas_total.

Actividad Transparencia 39 (Pag 49)
 Se compila con g++ -std=c++11 -pthread -Wfatal-errors -I. -o prodcons2_sc.out prodcons2_sc.cpp 

    -He realizado el prodcons2_sc pero me funciona perfectamente  tanto con while y con if. Asi que no se si es que no lo tendre bien, pero a priori parece que funciona de forma adecuada

Actividad Transparencia 51 (Pag 63)

    -El orden de salida se corresponde siempre con el de entrada, ya que cuando entra la ultima cita de ese grupo, empieza a hacer signal. Cada vez que realiza un signal, 
        el monitor manda a la cola de espera urgente a esa hebra y saca de la cola "normal" a la primera cita que se añadio, haciendo que la ultima hebra en llegar no pueda acabar hasta que realiza todos los signal.

    -Ninguna otra cita logra entrar hasta que acaba el grupo completo porque la exclusion mutua siempre la tendra el grupo hasta que acaben todos, debido a que cuando se saca un elemto de la cola y termina, 
        inmediatamente se devuelve el control a la hebra que ejecuta el signal. Haciendo que hasta que no realize todos los signals y ella misma acabe( que sera la ultima), no se cede el control a ninguna otra hebra.

Actividad Transparencia 52 (Pag 66)

LIFO
Se compila con g++ -std=c++11 -pthread -Wfatal-errors -I. -o prodcons2_su.out prodcons2_su.cpp  HoareMonitor.cpp

FIFO
Se compila con g++ -std=c++11 -pthread -Wfatal-errors -I. -o prodcons2_su_FIFO.out prodcons2_su_FIFO.cpp  HoareMonitor.cpp

Con semantica SU se puede usar un if en vez del while. Esto se debe a que cada vez va a sacar solo al que le toca, no va a sacar mas ni a otro que no sea su turno. Ya que tengo una CondVar para cada fumador.
