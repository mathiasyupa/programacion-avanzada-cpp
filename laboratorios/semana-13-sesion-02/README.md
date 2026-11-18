# Semana 13, Sesión 2: Concurrencia II: atomics y patrones seguros

## Objetivo de la sesión

Distinguir cuándo un tipo atómico reemplaza correctamente a un mutex y cuándo no, implementando un contador concurrente con std::atomic.

## Antes de empezar

**Lectura previa:** ["Beyond the Mutex: A Practical Guide to High-Performance Concurrency in C++", Mansi Maheshwari](https://medium.com/@mansi_maheshwari/beyond-the-mutex-a-practical-guide-to-high-performance-concurrency-in-c-a73a0755293e)

**Referencia:** [std::atomic (cppreference)](https://en.cppreference.com/w/cpp/atomic/atomic.html)

Según el artículo, ¿qué cuesta un mutex, y qué alternativas propone? ¿En qué casos dice que conviene seguir con el mutex?

_(tu respuesta)_

En la sesión pasada te pregunté: con 16 hilos y un millón de visitas cada uno, ¿dónde pasa el tiempo el programa? ¿Hace falta un candado completo para sumar 1 a un número?

_(tu respuesta)_

Entra a esta carpeta y crea la carpeta de los ejecutables:

```
cd laboratorios/semana-13-sesion-02
mkdir -p bin
```

## Ejercicio 1: el contador con std::atomic

Archivos: [`ejercicio1_contador_atomico.cpp`](./ejercicio1_contador_atomico.cpp), con dos `TODO`, y [`ejercicio1_contador_candado.cpp`](./ejercicio1_contador_candado.cpp), la versión con mutex de la sesión pasada, completa, con cuatro puertas. Compila los dos y corre cada uno con `time` antes de cambiar nada.

**Respuesta 1 (después del TODO 1):** `total` ya es un `std::atomic<int>`, y `registrar` sigue diciendo `total = total + 1;`. ¿Qué crees que imprime?

No esta garantizado que imprima 4000000. Aunque las lecturas y escrituras individuales sean atomicas, dos hilos pueden leer el mismo total y guardar el mismo resultado. En dos corridas dio 1404486 y 1065842.

**Respuesta 2 (después del TODO 2):** con `total++`, ¿qué imprime? ¿Por qué `total++` sí funciona y `total = total + 1` no, si `total` es atómico en los dos casos?

Con `total++` imprime siempre 4000000. El operador `++` de `std::atomic` es una sola operacion atomica de leer-modificar-escribir; en cambio, `total = total + 1` hace una lectura atomica y una escritura atomica separadas, con espacio para que otro hilo cambie el valor entre ambas.

**Respuesta 3:** compara el tiempo de la versión atómica con el de la versión con mutex. ¿Cuál es más rápida, y por cuánto?

En esta maquina la version atomica tardo 0.06 s y la de mutex 0.18 s: la atomica fue aproximadamente tres veces mas rapida. El valor exacto puede variar segun la maquina y lo que este ejecutandose.

**La versión completa, tal como la resuelvo yo:**

Se cambio `int total` por `std::atomic<int>` y `total = total + 1` por `total++`. La salida fue `Visitas registradas: 4000000`. En la medicion, la version atomica tardo 0.06 s y la version con candado 0.18 s.

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

## Ejercicio 2: la tarjeta del comedor sin mutex

Archivo: [`ejercicio2_tarjeta_atomica.cpp`](./ejercicio2_tarjeta_atomica.cpp). Solo o en pareja.

Alguien cambió el mutex de la tarjeta por `std::atomic<int>`. `cobrarIngenuo` es ese cambio, y cada una de sus líneas es atómica.

1. Corre el archivo tal cual tres veces. ¿Cuántas tarjetas quedaron en negativo con `cobrarIngenuo`? Córrelo también con `-fsanitize=thread`: ¿reporta algo?

En tres corridas quedaron 4, 49 y 98 tarjetas en negativo. ThreadSanitizer no reportaria una data race en `cobrarIngenuo`, porque los accesos a cada variable son atomicos; el problema es que varias operaciones atomicas no forman una transaccion unica.

2. Escribe `cobrar` con `compare_exchange_weak`, siguiendo los pasos del `TODO`. Corre diez veces y con ThreadSanitizer.
3. Responde: ¿por qué `cobrarIngenuo` deja tarjetas en negativo si cada línea es atómica? ¿Por qué ThreadSanitizer no lo detecta?

Dos cajas pueden leer un saldo de 1, decidir que alcanza y despues restar una por una, hasta llegar a -1. Cada lectura, resta o contador es atomico, pero la condicion y la resta no se ejecutan como una operacion indivisible. ThreadSanitizer busca carreras de memoria; aqui no hay accesos no atomicos en conflicto, sino un error de logica concurrente.

4. Responde: en `cobrar`, el saldo se cobra con `compare_exchange_weak`, y después se suma 1 a `aceptados` con otra operación. Si otro hilo leyera `saldo` y `aceptados` justo entre las dos, ¿qué vería? ¿Es un problema para este programa? ¿Para cuál sí lo sería?

Podria ver el saldo ya disminuido y el contador de aceptados todavia sin aumentar. No es problema aqui porque los resultados se leen despues de los `join()`. Si una pantalla o un reporte necesitara mostrar saldo y contadores como una fotografia coherente mientras se cobra, necesitaria una sincronizacion conjunta, por ejemplo un mutex.

## Ejercicio 3: el centro de impresión

Archivo: [`ejercicio3_centro_impresion.cpp`](./ejercicio3_centro_impresion.cpp). Solo o en pareja.

Cuatro computadoras envían trabajos al mismo tiempo, y cada estudiante tiene una cuota de 100 páginas. El encabezado describe la clase `CentroDeImpresion`; la escribes tú. El `main` ya está escrito. Mientras no escribas la clase, el archivo no compila.

1. Escribe `CentroDeImpresion` y corre el programa diez veces y con ThreadSanitizer: la salida tiene que ser siempre la del encabezado.
2. Completa la tabla con tu decisión para cada dato de la clase:

| Dato | ¿`std::atomic` o protegido por `std::mutex`? | Por qué |
|---|---|---|
| Si el centro está cerrado | `std::atomic<bool>` | Es un estado independiente que solo se lee o se cambia. |
| Trabajos aceptados y rechazados | `std::atomic<int>` | Son contadores que se incrementan de uno en uno. |
| Páginas totales del centro | `std::atomic<int>` | Es un acumulador simple de paginas aceptadas. |
| Páginas de cada estudiante (el `map`) | `std::mutex` | Hay que consultar la cuota y actualizar el mismo estudiante como una sola seccion critica; ademas, modificar un `map` no es seguro entre hilos. |

3. Responde: ¿por qué las páginas de cada estudiante no pueden ser un `std::map<int, std::atomic<int>>` sin mutex, aunque cada valor sea atómico? Piensa en la cuota y en lo que hace `[]` sobre un `map` cuando la clave no existe.

Aunque cada valor fuera atomico, la consulta de la cuota y su incremento tendrian que ocurrir juntos para no superar 100 paginas. Ademas, `map[estudiante]` puede crear una clave nueva, y cambiar la estructura interna del `map` desde varios hilos no es seguro sin mutex.

## Qué construcciones nuevas usa este laboratorio

`#include <atomic>`, `std::atomic<int>` y `std::atomic<bool>` como atributos, asignados en el constructor. Operaciones atómicas: leer (`int actual = saldo;`), asignar, `++`, `+=`, `-=`, `fetch_add`, y `compare_exchange_weak(esperado, nuevo)`. Medir el tiempo de un programa con `time`.

## Antes de la próxima sesión

**Lectura:** ["Why C++ Still Dominates High-Frequency Trading (And It's Not Just Speed)", Rishi Jain](https://medium.com/@beingrishijain/why-cpp-still-dominates-high-frequency-trading-and-its-not-just-speed-ad15d6a1a256)

Hoy la versión atómica del contador fue varias veces más rápida que la del mutex. En un sistema que compra y vende acciones, una diferencia de microsegundos decide quién llega primero. ¿Qué características de C++ que vimos en el semestre crees que importan en ese tipo de sistema, además de la velocidad?

_(tu respuesta)_
