#Filosofos sin camarero
-Aspectos destacados
Los filosofos antes de comer realizan dos mpi_Ssend, primero al proceso del tenedor de la izquierda  y luego al tenedor
de la derecha, despues empieza a comer y suelta los dos tenedores, esto se realiza en la funcion_filosofos.
La funcion de cada tenedor lo unico que hace es recibir primero la peticion de coger tenedor y despues la peticion de soltar
el tenedor
En el main se distingue cuales son tenedores y cuales filosofos

-¿Porque sucede el interbloqueo?
Una secuencia de interbloqueo puede ser cuando todos han codigo su tenedor izquierdo y ninguno ha codigo aun el derecho.
Entonces cuando uno intenta cogerlo, se encuentra que esta ocupado por otro filosofo, y asi le pasa a todos.

-¿Como se puede solucionar?
Esto se puede solucionar haciendo que algun filosofo coga primero el tenedor de la derecha, haciendo asi que el filosofo
que tiene a su izquierda no se vaya a bloquear.

-Salida del programa.
antonio-Lenovo ॐ  ~/Documentos/git/SCD/Practica3:(9h17m|git@master!)
4271 ± mpirun.mpich -np 10 ./filosofos_exe                                                                                                                                    ✹ ✚ ✭
Filósofo 0 solicita ten. izq.1
Filósofo Filósofo 6 solicita ten. der.5
Filósofo 0 solicita ten. der.9
Ten. 1 ha sido cogido por filo. 0
Filósofo 2 solicita ten. der.1
Filósofo 8 solicita ten. der.7
Ten. 7 ha sido cogido por filo. 8
Filósofo 8 solicita ten. izq.9
4 solicita ten. der.3
Filósofo Ten. 3 ha sido cogido por filo. 4
4 solicita ten. izq.5
Filósofo Ten. 5 ha sido cogido por filo. 4
4 comienza a comer
Filósofo Ten. 9 ha sido cogido por filo. 0
0 comienza a comer
Filósofo 0 suelta ten. izq. 1
Filósofo 0 suelta ten. der. 9
Filosofo 0 comienza a pensar
Ten. 1 ha sido liberado por filo. 0
Ten. 1 ha sido cogido por filo. 2
Ten. 9 ha sido liberado por filo. 0
Ten. 9 ha sido cogido por filo. 8
Filósofo 2 solicita ten. izq.3
Filósofo 8 comienza a comer
Ten. 3 ha sido liberado por filo. 4
Ten. 3 ha sido cogido por filo. 2


#Filosofos con camarero
-Aspectosa destacados
El programa tendra en el proceso con el id->10 el proceso camarero. En el main se mirara primero si es el camarero y si no, ya 
se ve si es filosofo o tenedor.

La funcion de los tenedores no cambia en nada.

En la funcion de los fiolosofos se introducen los siguiente cambios:
    -Se añade primero un mpi_send que pide al camarero si se puede sentar y luego espera bloqueado en mpi_recv hasta que recibe 
    la confirmacion por parte del camarero.
    -Despues de soltar los dos tenedores, el filosofo pasa a levantarse, haciendo un mpi_Ssend a camarero para decirle que se va a 
    levantar y que reste uno al total de filosofos que hay en la mesa

En la funcion del camarero que se añade se realiza lo siguiente:
    -Primero mira la cantidad de filosofos sentados, para saber a cual peticiones debe atender. Si quedan sillas disponibles, pide cualquier
    peticion, en cambio si no las hay, solo pide peticiones con la etiqueta de levantarse.
    -Si la etiqueta que recibe es para sentarse, mira cual es el emisor, recibe la peticion y pasa a decirle que se puede sentar. A su vez
    aumenta la cantidad sentada en la mesa.
    -Si la etiqueta que recibe es para levantarse, recibe el mensaje y resta uno a la cantidad de gente sentada en la mesa.

-Salida del programa

antonio-Lenovo ॐ  ~/Documentos/git/SCD/Practica3:(9h17m|git@master!)
4272 ± mpirun.mpich -np 11 ./filosofos-cam_exe                                                                                                                                ✹ ✚ ✭
El filosofo 2pide sentarse. 
El filosofo El filosofo 6pide sentarse. 
4pide sentarse. 
El filosofo 8pide sentarse. 
El filosofo Filosofo 2 se sienta. Hay 1 filosofos sentados. 
Filosofo 4El filosofo 4 puede coger tenedores. 
Filósofo  se sienta. Hay 2 filosofos sentados. 
Filosofo 6 se sienta. Hay 3 filosofos sentados. 
Filosofo 8 se sienta. Hay 4 filosofos sentados. 
4 solicita ten. izq.5
El filosofo 6 puede coger tenedores. 
Filósofo 6 solicita ten. izq.7
Filósofo 6 solicita ten. der.5
El filosofo 8 puede coger tenedores. 
Filósofo 8 solicita ten. izq.9
Filósofo 8 solicita ten. der.7
Ten. 7 ha sido cogido por filo. 6Ten. 9 ha sido cogido por filo. 
8
2 puede coger tenedores. 
Filósofo 2 solicita ten. izq.3
Filósofo 4 solicita ten. der.3
Ten. 5 ha sido cogido por filo. 4
El filosofo 0pide sentarse. 
Filósofo Ten. 3 ha sido cogido por filo. 2
2 solicita ten. der.1
Filósofo Ten. 1 ha sido cogido por filo. 2
2 comienza a comer
Filósofo 2 suelta ten. izq. 3
Filósofo Ten. 3 ha sido liberado por filo. 2
Ten. 3 ha sido cogido por filo. 4
Filósofo 4 comienza a comer
Ten. 1 ha sido liberado por filo. 2
2 suelta ten. der. 1
El filosofo2 se levanta a pensar.
    