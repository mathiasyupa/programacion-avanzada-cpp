# Semana 5, Sesión 2: Punteros y memoria dinámica II

## Objetivo de la sesión

Diagnosticar y corregir fugas de memoria y punteros colgantes en código dado, distinguiendo un manejo correcto de uno incorrecto de memoria dinámica.

## Antes de empezar

**Lectura previa:** ["Memory Leaks and Dangling Pointers in C++", Debajyoti Chatterjee](https://debajyotichatterjee-88008.medium.com/memory-leaks-and-dangling-pointers-9b2ae80c2fe7)

**Referencia:** ["delete expression", cppreference](https://en.cppreference.com/w/cpp/language/delete.html)

Según el artículo, ¿en qué se diferencia un puntero colgante de una fuga de memoria? Las dos son formas de mal manejo de memoria dinámica, pero no son el mismo problema.

un puntero colgante apunta a memoria que ya no se puede usar. una fuga de memoria pasa cuando se reserva memoria y no se libera, por eso el programa la pierde.

La sesión pasada até la vida de la memoria reservada al ciclo de vida del objeto que la reserva, con RAII. ¿Qué pasaría si alguien más guardara un puntero a esa memoria, y siguiera usando ese puntero después de que el objeto dueño ya se destruyó?

ese puntero quedaria apuntando a memoria que ya no pertenece al objeto. usarlo puede dar un dato raro o hacer fallar el programa.

## Ejercicio 1: reproducir un puntero colgante

Archivo: [`ejercicio1_puntero_colgante.cpp`](./ejercicio1_puntero_colgante.cpp). `obtenerLecturaInsegura` devuelve `&lectura`, la dirección de una variable local.

**Respuesta 1, antes de ver la solución:** `lectura` es una variable local de `obtenerLecturaInsegura`. ¿Qué pasa con ella cuando la función termina?

cuando termina la funcion, lectura deja de existir porque era una variable local. la direccion que se devuelve ya no es segura.

**Respuesta 2:** antes de compilar, ¿qué advertencia esperas que dé el compilador sobre esta función?

espero una advertencia porque la funcion devuelve la direccion de una variable local.

**La formulación completa, tal como la resuelvo yo:**

el compilador aviso que se devolvia la direccion de una variable local. al usar el puntero, el resultado no es confiable y el programa puede fallar.

## Ejercicio 2: corregirlo

Archivo: [`ejercicio2_puntero_seguro.cpp`](./ejercicio2_puntero_seguro.cpp), mismo punto de partida que el Ejercicio 1.

**Respuesta 1, antes de ver la solución:** si esta función sí necesita devolver un puntero, ¿a qué debería apuntar ese puntero para que siga siendo válido después de que la función retorne?

debe apuntar a memoria creada con new, porque esa memoria sigue existiendo despues de que la funcion termina.

**Respuesta 2:** después de usar el puntero devuelto, ¿qué dos pasos evitan que alguien lo use por accidente después de liberado?

primero se usa delete para liberar la memoria. despues se asigna nullptr al puntero para dejar claro que ya no apunta a una direccion valida.

**La formulación completa, tal como la resuelvo yo:**

la funcion crea un entero con new y devuelve su direccion. despues de mostrar la lectura, main usa delete y asigna nullptr. la salida fue lectura: 20.

## Durante el ConcepTest

**Tu voto, antes de discutir en pareja** (A, B, C o D):

no aplica porque la pregunta del conceptest no aparece en este readme.

**Tu razonamiento:**

no puedo dar un razonamiento sobre el voto porque no esta la pregunta del conceptest en el readme.

**Después de discutir en pareja, ¿cambiaste de voto? ¿Por qué?**

no aplica porque no se incluyo la pregunta ni las opciones del conceptest en el readme.

## Ejercicio 3: diagnosticar un inventario

Archivo: [`ejercicio3_inventario.cpp`](./ejercicio3_inventario.cpp). Solo o en pareja. El archivo compila y corre, pero tiene dos bugs marcados con `TODO`.

**Bug 1, en `contarProductosAgotados`:** el arreglo `agotados` se reserva con `new[]` y solo se usa dentro de la función. Identifica qué falta antes de que la función termine, y corrígelo.

**Bug 2, en `obtenerReporteFinal`:** la función devuelve la dirección de `totalUnidades`, una variable local. Corrígelo, revisando primero si de verdad hace falta devolver un puntero para esto. Si cambias la función para que devuelva el entero por valor, actualiza también `main`: ya no necesitas `int* totalPtr`, ni el `delete` sobre él.

El archivo tiene, en su encabezado, la salida exacta que debe producir cuando esté completo.

## Compilar y ejecutar

```
g++ -std=c++20 -Wall -Wextra -g ejercicio1_puntero_colgante.cpp -o bin/ejercicio1
./bin/ejercicio1
```

Repite el mismo patrón con `ejercicio2_puntero_seguro.cpp` y `ejercicio3_inventario.cpp`.

## Qué construcciones nuevas usa este laboratorio

`nullptr`: valor especial que indica que un puntero no apunta a memoria válida, útil para dejar constancia explícita después de un `delete` y para comprobar un puntero antes de usarlo (`if (puntero != nullptr)`). El vocabulario de punteros, `new`, `delete` y RAII es el mismo de la Semana 5, Sesión 1. Nada de listas de inicialización de miembros, `smart pointers` ni excepciones: eso llega en sesiones posteriores.

## Antes de la próxima sesión

Cada vez que una función de este laboratorio necesitó devolver algo, copiar ese algo (un `int`, un `double`) fue barato. ¿Por qué copiar un objeto grande (por ejemplo, un arreglo de un millón de elementos) sería mucho más caro que copiar un `int`, y qué alternativa a copiar se te ocurre?

copiar un arreglo grande tarda mas y usa mas memoria porque se deben copiar todos sus elementos. una alternativa es usar una referencia o un puntero y tener claro quien es responsable de esa memoria.
