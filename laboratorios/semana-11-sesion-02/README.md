# Semana 11, Sesión 2: Tipado moderno y manejo de errores explícito

## Objetivo de la sesión

Refactorizar una función que retorna códigos de error hacia el uso de std::optional, y justificar por qué reduce el riesgo de acceso nulo.

## Antes de empezar

**Lectura previa:** ["Optional Is Not Optional in C++ (and Definitely Not a Pointer)", Kyrylo Rud](https://medium.com/@kyrylo.rud/optional-is-not-optional-in-c-and-definitely-not-a-pointer-a1074f160c3e)

**Referencia:** ["std::optional", cppreference](https://en.cppreference.com/w/cpp/utility/optional.html)

Según el artículo, ¿por qué un `std::optional` "no es un puntero"? ¿Dónde vive el valor que guarda?

optional no guarda una direccion a otro objeto: contiene espacio para el
valor dentro del propio optional, junto con el estado de si hay valor o no.

Una función `buscarTemperatura(registro, hora)` no encuentra lectura para una hora. ¿Lanzarías una excepción? ¿Devolverías -1? ¿Y si la cámara de frío registra temperaturas bajo cero?

No encontrar una lectura es una ausencia esperada, asi que devolveria un
optional vacio. No devolveria -1 porque una temperatura bajo cero puede
ser una lectura valida de una camara de frio.

## Ejercicio 1: la cámara de frío

Archivo: [`ejercicio1_camara_de_frio.cpp`](./ejercicio1_camara_de_frio.cpp). Córrelo una vez antes de cambiar nada.

**Respuesta 1 (antes del TODO 1):** el registro tiene lectura a las 7. ¿Qué imprime el programa para la hora 7, y por qué?

Imprime "Hora 7: sin lectura" porque la lectura real es -1.0 y el codigo
anterior usaba -1 como señal de que no habia dato, confundiendo un valor
valido con la ausencia de valor.

**Respuesta 2 (TODO 1 y 2):** con `std::optional<double>` como tipo de retorno, ¿qué pasa si dejas `double temperatura = buscarTemperatura(registro, horas[i]);` sin cambiarla? ¿Es un error al compilar o al ejecutar?

Es un error de compilacion: no se puede convertir implicitamente un
optional<double> en double. Primero hay que comprobar que contiene valor y
despues leerlo con * o usar value_or.

**Respuesta 3 (TODO 3 y 4):** `value_or(4.0)` y `.value()` sobre la hora 8, que no tiene lectura. ¿Qué devuelve cada uno?

value_or(4.0) devuelve 4.0. value() no devuelve una temperatura: lanza
std::bad_optional_access porque el optional esta vacio.

**La versión completa, tal como la resuelvo yo:**

La firma es `std::optional<double> buscarTemperatura(...)`. Se revisa con
`if (temperatura)` antes de usar `*temperatura`. La salida final es:

```
Hora 6: 2.5
Hora 7: -1
Hora 8: sin lectura
Hora 9: -3.5
Hora 8 para el reporte: 4
Hora 8 con value(): bad optional access
```

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

## Ejercicio 2: el catálogo sin punteros

Archivo: [`ejercicio2_catalogo_optional.cpp`](./ejercicio2_catalogo_optional.cpp). Solo o en pareja.

`buscar` devuelve un puntero al producto dentro del vector, o `nullptr`. Corre el archivo sin cambios: termina antes de tiempo. Resuelve los tres `TODO`: `buscar` devuelve `std::optional<Producto>`, `main` revisa cada resultado, y el descuento de un producto pasa a ser un atributo `std::optional<double>`. La salida esperada exacta está en el encabezado.

Antes de resolverlo, compila y corre la versión sin cambios con `-fsanitize=address`. ¿Qué dos problemas distintos reporta, y en qué líneas?

Reporta un uso despues de liberar memoria al usar `encontrado` en la linea
83: agregar productos puede realojar el vector y dejar ese puntero viejo.
Tambien reporta acceso nulo en la linea 88 al buscar el producto 999 y
usar el puntero nulo. En esta computadora AddressSanitizer no esta
disponible por una biblioteca faltante, pero esos son los dos errores que
detectaria.

Antes de dar el ejercicio por terminado, responde: `encontrado` se busca antes de agregar dos productos al catálogo. ¿Por qué con el puntero eso era un problema y con el `std::optional` ya no lo es?

El puntero apuntaba a un elemento interno del vector y podia quedar
colgando si push_back realojaba el vector. El optional guarda una copia del
Producto encontrado, asi que no depende de la memoria interna del vector.

## Ejercicio 3: el estacionamiento de la facultad

Archivo: [`ejercicio3_estacionamiento.cpp`](./ejercicio3_estacionamiento.cpp). Solo o en pareja.

`asignarPuesto`, `puestoDe` y `liberar` devuelven -1 o -2 en vez de un número de puesto, y `main` imprime esos códigos como si fueran puestos. Refactoriza las tres funciones para que ninguna devuelva un código. Para cada caso, tú decides si es una ausencia esperada (`std::optional`) o un error de quien llama (una excepción), y ajustas `main` para producir la salida esperada exacta del encabezado.

Antes de dar el ejercicio por terminado, completa la tabla con tu decisión para cada caso:

| Función | Caso | `std::optional` o excepción | Por qué |
|---|---|---|---|
| `asignarPuesto` | No hay puestos libres | `std::optional` | Es una situacion posible y quien llama puede decidir que informar. |
| `asignarPuesto` | La placa ya está adentro | excepción | Es un uso invalido: se intenta registrar dos veces la misma placa. |
| `puestoDe` | La placa no está | `std::optional` | Una busqueda puede no encontrar un resultado. |
| `liberar` | La placa no está | `std::optional` | Puede ser normal intentar liberar una placa que ya no esta. |

Responde también: la placa 4521 queda en el puesto 0. Si escribes `if (puesto)` sobre un `std::optional<int>` que guarda 0, ¿entra al `if`? ¿Por qué eso no pasaba con el puntero ni con el código -1, y por qué con el `optional` es lo correcto?

Si entra: el bool de optional pregunta si hay valor, no si el entero es
distinto de cero. Con un puntero se revisaba si habia direccion y con el
codigo se comparaba contra -1; optional expresa directamente que el puesto
0 es un resultado presente y valido.

## Compilar y ejecutar

```
g++ -std=c++20 -Wall -Wextra -g ejercicio1_camara_de_frio.cpp -o bin/ejercicio1
./bin/ejercicio1
```

Repite el mismo patrón con `ejercicio2_catalogo_optional.cpp` y `ejercicio3_estacionamiento.cpp`. Para el sanitizer del Ejercicio 2:

```
g++ -std=c++20 -g -fsanitize=address ejercicio2_catalogo_optional.cpp -o bin/ejercicio2_asan
./bin/ejercicio2_asan
```

## Qué construcciones nuevas usa este laboratorio

`std::optional<T>` (de `<optional>`) como tipo de retorno y como atributo; `std::nullopt` para devolver un `optional` vacío; `has_value()` y la conversión a `bool` (`if (resultado)`) para preguntar si hay valor; `*resultado` y `resultado->metodo()` para usarlo; `value_or(valorPorDefecto)`; `value()`, que lanza `std::bad_optional_access` si está vacío; asignar un valor a un `optional` ya declarado (`descuento = fraccion;`).

## Antes de la próxima sesión

**Lectura:** ["The Illusion of Safety: Why Passing Tests Don't Guarantee Bug-Free Code", Tshegofatsoseane](https://medium.com/@tshegofatsoseane98/the-illusion-of-safety-why-passing-tests-dont-guarantee-bug-free-code-0ce44fd5c43b)

En este laboratorio, cada ejercicio trae su salida esperada y la comparas a ojo con la que imprime tu programa. ¿Qué tendría que hacer un programa para comparar esa salida por ti, cada vez que cambias el código? ¿Qué no podría detectar aunque todas las líneas coincidan?

Un programa de pruebas tendria que ejecutar el ejercicio y comparar su
salida con una salida esperada automaticamente. Aun asi no detectaria
errores en casos no probados, fugas de memoria o una implementacion que
solo coincide con esos datos concretos.
