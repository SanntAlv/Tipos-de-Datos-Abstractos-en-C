# TDA HASH


## Repositorio de Santiago Alvarez - 111935 - saalvarez@fi.uba.ar


- Para compilar:


```bash
gcc tp_hash.c src/csv.c src/hash.c
```


- Para ejecutar:


```bash
./a.out ejemplos/pokedex.csv
```


- Para ejecutar con valgrind:
```bash
valgrind ./a.out ejemplos/pokedex.csv
```
---
##  Qué es un diccionario:


Un diccionario es una estructura que permite acceder a un dato por medio de una clave


Un diccionario tiene dos elementos fundamentales, la clave y el valor asociado a la misma, donde la clave funciona como un índice para buscar el valor o dato relacionado.


Hay que tener en cuenta que en el contexto de un diccionario no siempre tiene sentido que la clave se repita, es decir, debe ser única. El elemento asociado a la clave si puede repetirse.


<div align="center">
<img width="50%" src="img/clavevalor.svg">
</div>




Algunos ejemplos de diccionarios pueden ser:


1. ***Tabla de Hash***: Este tipo de implementación transforma una clave en un índice numérico (usando una función de Hash) para almacenar el valor en una posición en concreto de una estructura en concreto. Lo interesante es que la única forma de acceder al valor es utilizando la función de hash para **"transformar"** la clave, que puede ser una palabra por ejemplo, en un indice numérico.


2. ***ABB***: Un árbol binario de búsqueda se compone de nodos, donde cada nodo (en el caso de un diccionario) tiene una clave y un valor. En el ABB se comparan las claves siguiendo un criterio en específico para ser hijos derechos o izquierdos del nodo anteriormente insertado.


3. ***Lista enlazada***: Igual que la implementación de un ABB, se compone de nodos donde cada nodo tiene una clave y un valor. Entonces cuando se recorre la lista enlazada se puede acceder a un valor en especifico según su clave.


---


## Qué es una función de hash:


Una ***función de hash*** transforma una clave (cualquiera sea esta) en un valor numérico (que para un diccionario puede ser interpretada como un índice). Para un mismo input, siempre que se aplique la misma función de Hash, se obtiene el mismo número asociado.


<div align="center">
<img width="100%" src="img/tablahash.svg">
</div>




Estas funciones tienen además el objetivo de distribuir de la forma más uniforme los resultados numéricos para evitar que dos claves distintas nos den el mismo número asociado, en este caso se genera una colisión dentro de la tabla.




## Qué es una tabla de Hash:


Una tabla de hash es una estructura de datos que implementa un diccionario.


Permite almacenar y recuperar los pares clave valor usando una función Hash, la cual transforma la clave del par a un índice de una estructura para almacenar, buscar, eliminar un dato asociado.


Existen distintos tipos de Hash según su direccionamiento y la forma en que se gestionan las colisiones (explicado anteriormente):


<div align="center">
<img width="80%" src="img/hashcerrado.svg">
</div>


1. ***Hash Abierto(direccionamiento cerrado)***: La tabla de hash es reducida a un array dinámico de punteros a estructuras de datos. Obviamente, el array tiene un índice, este índice se calcula dada una clave y su transformación por medio de una función de hash. Entonces se accede al índice en cuestión del array y el valor asociado es almacenado en la estructura que es apuntada por el array. Esta estructura puede ser una lista enlazada, un árbol binario, un vector o lo que sea
De forma que, cuando ocurre una colisión (se obtiene un mismo índice para una clave distinta) entonces se inserta en la estructura previamente definida. Este método se conoce como ***"encadenamiento"*** o ***"chaining"*** y es un direccionamiento cerrado porque el valor asociado a la clave siempre va al mismo índice.


<div align="center">
<img width="80%" src="img/encadenamiento.svg">
</div>




2. ***Hash Cerrado (direccionamiento abierto)***: Que el hash sea cerrado significa que todos los valores son almacenados dentro de una misma estructura, la Tabla Hash. Para la explicación supongamos que esta estructura es un array dinámico, entonces los valores se insertan de forma instantánea en el índice que corresponde a su clave según una función de hash. Cuando ocurre una colisión, entonces se usa alguna de las siguientes políticas para decidir dónde se coloca el valor.


<div align="center">
<img width="50%" src="img/hashabierto.svg">
</div>






2.1. ***Probin lineal***: Busca el siguiente espacio inmediato y libre e inserta el valor en dicha posición.


2.2 ***Probing cuadrático***: Eleva al cuadrado la cantidad de veces que hubo colisión, es decir, hay una colisión, entonces te moves 1² posiciones, si esta posición está ocupada, entonces se mueve 2² posiciones más. Así hasta que encuentra una posición libre


2.3 ***Hash doble***: Aplica una segunda función de hash a la clave.


<div align="center">
<img width="85%" src="img/probinglineal.svg">
</div


---


## Porque es importante el tamaño de la tabla:


**Caso de Hash Cerrado**: A mayor cantidad de valores almacenados en una tabla, más chances tenemos de que ocurra una colisión. Además sabemos que en el direccionamiento abierto existe una posición del array de la tabla para cada clave. Cuando se llega al límite de este array, obligatoriamente hay que aumentar la capacidad de la tabla.


Generalmente se calcula un factor C =  X / Y, donde X es el número de claves almacenadas actualmente y Y es la capacidad de la tabla. Esto se hace con el objetivo de evitar llegar a la capacidad máxima de la tabla y que la cantidad de colisiones sea mínima.


Entonces nosotros podemos definir que, por ejemplo, cuando el factor es de 0.8 y se ingrese una clave se haga un rehash. Un rehash es aumentar el tamaño de la tabla (por ejemplo al doble) y volver a ingresar todas los elementos en la tabla.


¿Por qué se vuelven a ingresar todos los elementos en la tabla? Porque de este modo la tabla queda mucho mejor distribuida y es más probable evitar colisiones. La complejidad de rehashear es O(n), porque tenemos que crear una nueva tabla e ingresar todos los elementos de la tabla vieja a la nueva. Pero esto mejora la complejidad de las operaciones de la tabla a la larga siempre y cuando no se rehashe con demasiada frecuencia.


***Caso de Hash Abierto***: Inicialmente uno puede pensar que no es necesario rehashear en un direccionamiento cerrado porque los elementos siempre se van a poder insertar en una estructura, a pesar de que el valor asociado a la clave sea el mismo. Pero esto trae problemas, imaginemos que inicialmente nuestra tabla tiene una capacidad de 5, y como estructura de soporte usamos una lista enlazada, luego se insertan 50000 elementos en la tabla y suponiendo que la función de hash sea perfecta, entonces tendríamos 10000 elementos en cada tabla, por lo que la búsqueda de un elemento en concreto no sería ni de cerca O(1), quizá esto se podría mejorar usando un ABB, pero tampoco tendríamos un buen rendimiento.


Rehasehar es importante para ***"descongestionar"*** las estructuras y mejorar el tiempo de acceso a los elementos.


---
---

