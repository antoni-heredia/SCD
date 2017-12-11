#Problema productores consumidores, con multiples procesos.

He realizado cambios en casi todas las funciones.

En el main, se ve  de que tipo es el proceso. 
Lo principal del main, es que se van enviando el  numero de orden entre los distintos productores/consumidores.
Es decir el primero de cada(ya se productor o consumidor), empieza en 0 y este se encarga de enviar al siguiente
proceso de ese tipo su numero de orden, el siguiente recibe el suyo y le envia al siguiente el que le toca, esto 
se realiza hasta que se llega al ultimo, que solo lo recibe. Esto es igual tanto para consumidor que para productor.


La funcion de los productores, recibe el numero de orden de ese productor.
Este productor solo producira k_pro numeros. k_pro es la cantidad de items que le toca a cada productor, es decir num_items/np.
Una vez producido se lo envia al buffer

La funcion del buffer no recibe ningun parametro.
El primer cambio importante en el buffer es que en el switch se mira ahora el tag y no el source, ya que como hay muchos sources con el mismo tag 
nos da igual de cual venga.
Si el tag es de un productor, se recoge el valor y se añade  al vector.
Si el tag es de un consumidor, se le envia el contenido de una posicion del vector.

La funcion del consumidor cambia en que ahora solo se realizan k_consumir "consumiciones" por proceso. El envio de la peticion y la recepcion
del valor se realiza con la etiqueta de consumidor.


La salida completa del programa es:

Productor ha producido valor 0
Productor va a enviar valor 0
Productor ha producido valor Buffer ha recibido valor 0
1
Productor va a enviar valor 1
El productor Buffer ha recibido valor 1
0 envia el valor 1 al productor 1Productor ha producido valor 2
Productor va a enviar valor 2
Buffer ha recibido valor 2
Productor ha producido valor 3
Productor va a enviar valor 3
El productor 1 envia el valor 2 al productor 2Buffer ha recibido valor 3
Productor ha producido valor 4
Productor va a enviar valor 4
Productor ha producido valor 5
Productor va a enviar valor 5
El productor 2Buffer ha recibido valor 4
Buffer ha recibido valor 5
 envia el valor 3 al productor 3Buffer va a enviar valor Productor ha producido valor 6
Productor va a enviar valor 6
0
Buffer ha recibido valor Productor ha producido valor 7
Productor va a enviar valor 7
Consumidor ha recibido valor 0
6
Buffer ha recibido valor El productor 3 envia el valor 4 al productor 47Productor ha producido valor 8
Productor va a enviar valor 8

Productor ha producido valor 9
Productor va a enviar valor 9
Buffer ha recibido valor 8
El productor 4 envia el valor 5 al productor 5Buffer ha recibido valor Productor ha producido valor 109
Productor va a enviar valor 10

Productor ha producido valor 11
Productor va a enviar valor 11
Buffer ha recibido valor 10
Consumidor ha consumido valor 0
Buffer va a enviar valor 1
El productor 5 envia el valor 6 al productor 6Buffer ha recibido valor 11
Consumidor ha recibido valor 1
Productor ha producido valor 12
Productor va a enviar valor 12
Consumidor ha consumido valor 1
Buffer va a enviar valor 2
Consumidor ha recibido valor Buffer ha recibido valor 12
2
Productor ha producido valor 13
Productor va a enviar valor 13
Consumidor ha consumido valor 2
Buffer va a enviar valor 3
Consumidor ha recibido valor El productor 6Buffer ha recibido valor 13
 envia el valor 7 al productor 73
Productor ha producido valor 14
Productor va a enviar valor 14
Consumidor ha consumido valor 3
Buffer va a enviar valor 4
Buffer ha recibido valor 14
Consumidor ha recibido valor 4
Productor ha producido valor 15
Productor va a enviar valor 15
El productor 7 envia el valor 8 al productor 8Buffer va a enviar valor 5
Buffer ha recibido valor 15
Consumidor ha consumido valor 4
Consumidor ha recibido valor 5
Productor ha producido valor 16
Productor va a enviar valor 16
Buffer va a enviar valor 6
Buffer ha recibido valor 16
Consumidor ha consumido valor 5
Consumidor ha recibido valor 6
Productor ha producido valor 17
Productor va a enviar valor 17
Buffer va a enviar valor 7
Buffer ha recibido valor 17
Consumidor ha consumido valor 6
Consumidor ha recibido valor 7
El productor 8 envia el valor 9 al productor 9Productor ha producido valor 18
Productor va a enviar valor 18
Consumidor ha consumido valor 7
Buffer va a enviar valor 8
Buffer ha recibido valor 18
Consumidor ha recibido valor 8
Productor ha producido valor 19
Productor va a enviar valor 19
Buffer va a enviar valor 9
Buffer ha recibido valor 19
Consumidor ha consumido valor 8
Consumidor ha recibido valor 9
Buffer va a enviar valor 10
Consumidor ha consumido valor 9
El productor 11 envia el valor 1 al productor 12Consumidor ha recibido valor 10
Consumidor ha consumido valor 10
Buffer va a enviar valor 11
Consumidor ha recibido valor 11
Buffer va a enviar valor 12
Consumidor ha consumido valor 11
Consumidor ha recibido valor 12
Buffer va a enviar valor 13
Consumidor ha consumido valor 12
Consumidor ha recibido valor 13
Consumidor ha consumido valor 13
Buffer va a enviar valor 14
Consumidor ha recibido valor 14
Buffer va a enviar valor 15
Consumidor ha consumido valor 14
Consumidor ha recibido valor 15
Buffer va a enviar valor 16
Consumidor ha consumido valor 15
Consumidor ha recibido valor 16
Buffer va a enviar valor 17
Consumidor ha consumido valor 16
Consumidor ha recibido valor 17
Buffer va a enviar valor 18
Consumidor ha consumido valor 17
Consumidor ha recibido valor 18
Consumidor ha consumido valor 18
Buffer va a enviar valor 19
Consumidor ha recibido valor 19
