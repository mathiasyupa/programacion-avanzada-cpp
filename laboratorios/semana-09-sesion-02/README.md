# Semana 9, Sesión 2: Plantillas y STL I: templates

## Objetivo de la sesión

Escribir una función o clase genérica con templates que evite duplicar código para distintos tipos de datos.

## Antes de empezar

**Lectura previa:** ["Function and Class Templates in C++: A Practical Guide to Generic Programming", Murat Toğunçhan Düzgün](https://medium.com/@togunchan/function-and-class-templates-in-c-enhancing-flexibility-and-reusability-894bd3d0c84e)

**Referencia:** ["Templates", cppreference](https://en.cppreference.com/w/cpp/language/templates.html)

Según el artículo, ¿qué hace el compilador cuando usas una plantilla de función con un tipo que todavía no había visto? ¿Ocurre al compilar o al ejecutar?

El compilador genera una version de la funcion para ese tipo cuando se
necesita. Esto ocurre al compilar, no al ejecutar el programa.

Necesitas `mayor` para `int`, para `double` y para `char`, y escribes tres funciones con el mismo cuerpo. Si mañana encuentras un error en ese cuerpo, ¿cuántas veces lo corriges?

Hay que corregirlo en las tres funciones. Con una plantilla se corrige en
un solo lugar.

## Ejercicio 1: mayor y sumarTres

Archivo: [`ejercicio1_mayor_template.cpp`](./ejercicio1_mayor_template.cpp). Tres funciones `mayor` duplicadas y un `Vector2D` con `operator+`, el de la Semana 3.

**Respuesta 1, antes de ver la solución:** con una plantilla `mayor`, ¿compila `mayor(3, 4.5)`? Si no, ¿qué dice el error y cómo se corrige sin tocar la plantilla?

No compila porque el compilador deduce dos tipos distintos para T: int y
double. Se puede indicar el tipo con `mayor<double>(3, 4.5)`.

**Respuesta 2:** `sumarTres(v1, v2, v3)` compila con tres `Vector2D`. ¿Compila `mayor(v1, v2)`? ¿Qué parte de la plantilla decide si un tipo sirve o no?

No compila porque Vector2D tiene operator+ pero no tiene operator>. Las
operaciones usadas en el cuerpo de la plantilla deben estar disponibles
para el tipo que recibe.

**La formulación completa, tal como la resuelvo yo:**

La plantilla usa `template <typename T>` y devuelve el mayor comparando
`a > b`. Los errores fueron `no matching function for call to
mayor(int, double)` por los tipos distintos y `no match for operator>`
para Vector2D. La salida final es:

```
mayor(3, 7) = 7
mayor(2.5, 1.5) = 2.5
mayor('a', 'z') = z
mayor(3, 4.5) = 4.5
sumarTres(1, 2, 3) = 6
sumarTres(v1, v2, v3) = (6, 3)
```

## Durante el ConcepTest

**Tu voto, antes de discutir en pareja** (A, B, C o D):

_(tu voto)_

**Tu razonamiento:**

_(tu respuesta)_

**Después de discutir en pareja, ¿cambiaste de voto? ¿Por qué?**

_(tu respuesta)_

## Ejercicio 2: funciones sobre arreglos

Archivo: [`ejercicio2_funciones_arreglo.cpp`](./ejercicio2_funciones_arreglo.cpp). Solo o en pareja.

Escribe las plantillas de función `sumarArreglo(datos, cantidad)` y `contarMayoresQue(datos, cantidad, umbral)`. `main` ya está completo, y usa `sumarArreglo` con arreglos de `int`, `double` y `Vector2D`. La salida esperada exacta está en el encabezado del archivo.

Antes de dar el ejercicio por terminado, responde: ¿con qué valor arrancaste la suma? Si arrancas en `T total = 0;`, ¿con qué tipo deja de compilar, y por qué? ¿Qué condición nueva le impone tu versión al arreglo que recibe?

La suma comienza con `datos[0]`. Si se usa `T total = 0`, no compila con
Vector2D porque no se puede convertir un entero en ese tipo. Al empezar
con el primer elemento, el arreglo debe tener al menos un elemento.

## Ejercicio 3: Historial genérico

Archivo: [`ejercicio3_historial_template.cpp`](./ejercicio3_historial_template.cpp). Solo o en pareja.

Escribe la plantilla de clase `Historial<T>` a partir de la especificación del archivo: un arreglo de `T` en un `std::unique_ptr<T[]>`, con capacidad fija, `agregar`, `obtener`, `ultimo`, `getUsadas` y `getCapacidad`. `main` la usa con `int`, `double` y `char`.

Antes de dar el ejercicio por terminado, responde: ¿compilaría `Historial<Vector2D> posiciones(2);`? ¿Qué línea de tu plantilla lo decide?

No con el Vector2D de estos ejercicios: no tiene constructor sin
parametros. `std::make_unique<T[]>(capacidad)` necesita construir cada
elemento del arreglo y por eso necesita ese constructor.

## Compilar y ejecutar

```
g++ -std=c++20 -Wall -Wextra -g ejercicio1_mayor_template.cpp -o bin/ejercicio1
./bin/ejercicio1
```

Repite el mismo patrón con `ejercicio2_funciones_arreglo.cpp` y `ejercicio3_historial_template.cpp`. Para el Ejercicio 3, compila también con `-fsanitize=address` y confirma que no queda ninguna fuga.

## Qué construcciones nuevas usa este laboratorio

Plantilla de función (`template <typename T>` antes de la función, `T` como tipo de parámetros, variables y retorno), deducción de argumentos de plantilla y argumento explícito entre ángulos (`mayor<double>(3, 4.5)`), y plantilla de clase (`template <typename T> class Historial { ... };`, creada con el tipo explícito, `Historial<int> pasos(5);`).

## Antes de la próxima sesión

**Lectura:** ["STL Containers: Vector, Set and Map", Megha B H](https://medium.com/@megha_bh/stl-containers-vector-set-and-map-7a90e0b1e19f)

`Historial` tiene una capacidad fija y rechaza el valor que no cabe. ¿Qué tendría que hacer para crecer cuando se llena? ¿Qué te dice la lectura sobre quién ya resolvió ese problema en la biblioteca estándar?

Tendria que reservar un arreglo mas grande, pasar los valores existentes y
liberar el anterior. `std::vector` ya permite crecer de forma automatica.
