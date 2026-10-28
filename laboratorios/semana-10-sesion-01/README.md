# Semana 10, Sesión 1: Plantillas y STL II: contenedores e iteradores

## Objetivo de la sesión

Elegir el contenedor de la STL adecuado según el patrón de acceso e inserción que exige un problema dado.

## Antes de empezar

**Lectura previa:** ["STL Containers: Vector, Set and Map", Megha B H](https://medium.com/@megha_bh/stl-containers-vector-set-and-map-7a90e0b1e19f)

**Referencia:** ["Containers library", cppreference](https://en.cppreference.com/w/cpp/container.html)

Según el artículo, ¿qué diferencia a un `set` de un `vector` cuando insertas un valor que ya estaba? ¿Y en qué orden quedan los elementos de un `map` al recorrerlo?

Un set no guarda el valor repetido, mientras que un vector si permite
repeticiones. Al recorrer un map, los elementos aparecen ordenados por su
clave.

`Historial` tiene una capacidad fija y rechaza el valor que no cabe. ¿Qué tendría que hacer para crecer cuando se llena?

Tendria que reservar un arreglo mas grande, copiar o mover los valores y
liberar el arreglo anterior. Un vector realiza ese crecimiento por si
solo al usar push_back.

## Ejercicio 1: Historial sobre un vector

Archivo: [`ejercicio1_historial_vector.cpp`](./ejercicio1_historial_vector.cpp). El `Historial<T>` de la sesión pasada, con capacidad 3, recibe 6 valores.

**Respuesta 1, antes de ver la solución:** con un `std::vector<int>` y `reserve(3)`, ¿qué capacidad esperas ver después de agregar el cuarto valor? ¿Qué tuvo que pasar con los tres valores que ya estaban guardados?

La capacidad debe crecer a por lo menos 4; en esta ejecucion crecio de 3 a
6. El vector reservo una zona nueva y paso los tres valores existentes a
esa nueva zona.

**Respuesta 2:** en el paso 4, `semana.end()` no apunta al último elemento. ¿A qué apunta, y por qué el loop se detiene con `it != semana.end()` en vez de `it <= semana.end()`?

end() apunta a la posicion inmediatamente posterior al ultimo elemento y
no se puede desreferenciar. El iterador se detiene al llegar a esa marca;
un iterador no usa <= y comparar asi no es valido para todos los
contenedores.

**La formulación completa, tal como la resuelvo yo:**

Historial usa un std::vector<T>, reserva la capacidad inicial con reserve
y agrega con push_back. Las capacidades observadas fueron 3, 3, 3, 6, 6
y 6. La salida del paso 4 es:

```
Semana: 4200 1800 3300 5100 2700 3900 4600
Total de la semana: 25600
```

## Durante el caso comparativo

Una fila de impresión recibe trabajos urgentes al frente y cancela trabajos en cualquier posición. Un registro de lecturas solo agrega al final y se consulta por posición. ¿Qué contenedor usarías para cada uno, y qué operación decide la elección?

Usaria una list para la fila, porque insertar al frente y borrar un trabajo
localizado no desplaza los demas elementos. Usaria un vector para las
lecturas, porque agrega al final y permite consultar por indice.

## Durante los ConcepTests

**ConcepTest 1. Tu voto, antes de discutir en pareja** (A, B, C o D):

_(tu voto)_

**Tu razonamiento:**

_(tu respuesta)_

**Después de discutir en pareja, ¿cambiaste de voto? ¿Por qué?**

_(tu respuesta)_

**ConcepTest 2. Tu voto, antes de discutir en pareja** (A, B, C o D):

_(tu voto)_

**Tu razonamiento:**

_(tu respuesta)_

## Ejercicio 2: la fila de la impresora

Archivo: [`ejercicio2_cola_impresion.cpp`](./ejercicio2_cola_impresion.cpp). Solo o en pareja.

Completa los métodos de `ColaDeImpresion`, que guarda los trabajos en una `std::list<int>`. `main` ya está completo y la salida esperada exacta está en el encabezado del archivo.

Antes de dar el ejercicio por terminado, responde: en `cancelar`, ¿qué pasaría si después de `trabajos.erase(it)` siguieras el loop con `++it`? ¿Qué operación de esta clase sería más barata con un `std::vector`, y por qué igual conviene la lista para este problema?

erase invalida ese iterador, asi que incrementarlo seria un comportamiento
indefinido. Agregar al final suele ser muy eficiente con vector por su
memoria contigua, pero la lista conviene porque insertar urgentes al frente
y cancelar en medio no obliga a desplazar todos los trabajos posteriores.

## Ejercicio 3: la bodega de componentes

Archivo: [`ejercicio3_bodega.cpp`](./ejercicio3_bodega.cpp). Solo o en pareja.

Escribe la clase `Bodega` a partir de la especificación del archivo. La especificación no dice qué contenedor usa cada atributo: lo eliges tú. `main` ya está completo y la salida esperada exacta está en el encabezado.

Antes de dar el ejercicio por terminado, responde por cada atributo: ¿qué contenedor elegiste, qué operación de la especificación decidió esa elección, y qué se rompería o se volvería más caro con otro contenedor?

_(stock)_

Se uso un map<int, int> porque el stock se consulta por codigo y debe
imprimirse ordenado por codigo. Un vector requeriria buscar el codigo de
forma lineal o reservar posiciones muy grandes.

_(historial de retiros)_

Se uso un vector<int> porque guarda cada retiro en orden, permite
repeticiones y permite consultar retiroNumero(posicion). Un set perderia
las repeticiones y una list no permite acceso directo por posicion.

_(agotados)_

Se uso un set<int> porque no admite codigos repetidos y los recorre de
menor a mayor. Un vector exigiria comprobar manualmente si el codigo ya
existia y ordenar antes de imprimir.

Responde también: ¿por qué `stockDe(999)` no puede escribirse como `return stock[999];` si `stock` es un `map`? ¿Qué línea de la salida esperada lo delataría?

El operador [] crea la clave 999 si no existe, asi que consultarla la
registraria como producto. La linea "Productos registrados: 3" cambiaria
a 4.

## Compilar y ejecutar

```
g++ -std=c++20 -Wall -Wextra -g ejercicio1_historial_vector.cpp -o bin/ejercicio1
./bin/ejercicio1
```

Repite el mismo patrón con `ejercicio2_cola_impresion.cpp` y `ejercicio3_bodega.cpp`.

## Qué construcciones nuevas usa este laboratorio

Los contenedores `std::vector<T>` (`push_back`, `size`, `[]`, `back`, `reserve`, `capacity`, inicialización con llaves), `std::list<T>` (`push_front`, `push_back`, `front`, `pop_front`, `erase`), `std::set<T>` (`insert`, `count`) y `std::map<K, V>` (`[]`, `count`, `size`, recorrido con `.first` y `.second`); iteradores (`begin()`, `end()`, `*it`, `++it`); `auto` para el tipo de un iterador; y el `for` de rango (`for (int valor : semana)`).

## Antes de la próxima sesión

**Lectura:** ["C++ STL: Overview of <algorithm>", Michael Spagna](https://mjspagna.medium.com/c-stl-overview-of-algorithm-a0d94342253e)

Hoy recorriste un `vector` y una `list` con el mismo patrón de iterador. Si mañana necesitas sumar, buscar el máximo y contar valores en los dos contenedores, ¿cuántas funciones escribirías con lo que sabes hoy? ¿Qué tendría que recibir una sola función para servir a los dos?

Con lo aprendido hasta hoy escribiria versiones separadas para cada
operacion y contenedor. Una sola funcion generica tendria que recibir el
inicio y el final del recorrido, es decir, dos iteradores.
