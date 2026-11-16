# Semana 13, Sesión 1: Concurrencia I: threads, mutex, condiciones de carrera

## Objetivo de la sesión

Identificar una condición de carrera en código concurrente y protegerla con un mutex.

## Antes de empezar

**Lectura previa:** ["Race Condition: The Invisible Bug That Breaks Your Code", Varbolean](https://medium.com/@varboleansh/race-condition-c5d038870f83)

**Referencia:** [std::mutex (cppreference)](https://en.cppreference.com/w/cpp/thread/mutex.html)

Según el artículo, ¿qué es una condición de carrera, y por qué es tan difícil de encontrar probando el programa?

_(tu respuesta)_

En la sesión pasada te pregunté por un programa que falla una de cada mil ejecuciones con los mismos datos. ¿Qué tendría que estar pasando? ¿Te serviría gdb para encontrarlo?

_(tu respuesta)_

Entra a esta carpeta y crea la carpeta de los ejecutables:

```
cd laboratorios/semana-13-sesion-01
mkdir -p bin
```

Los tres programas usan `<thread>` y `<mutex>`, que ya vienen con el compilador. Cada archivo trae su comando de compilación en el encabezado, y el de ThreadSanitizer (`-fsanitize=thread`), el sanitizer de la Semana 12 para hilos.

## Ejercicio 1: el contador de visitas de la biblioteca

Archivo: [`ejercicio1_contador_visitas.cpp`](./ejercicio1_contador_visitas.cpp). Compílalo y córrelo una vez antes de cambiar nada: las dos puertas registran una después de la otra, y la cuenta da bien.

**Respuesta 1 (antes del TODO 1):** con las dos puertas en dos hilos, al mismo tiempo, cada una registra un millón de visitas. ¿Qué número crees que imprime? ¿El mismo cada vez?

No hay un numero garantizado. Puede imprimir menos de 2000000 y cambiar en cada corrida, porque los dos hilos cambian `total` al mismo tiempo sin coordinacion.

**Respuesta 2 (después del TODO 1):** corre el programa cinco veces y anota los cinco resultados. `total = total + 1` es una sola línea. ¿Por qué se pierden visitas?

Sin mutex obtuve: 1159453, 1227133, 1259348, 1223223 y 1215545. La linea parece una operacion, pero primero lee el valor, luego suma y despues lo escribe. Dos hilos pueden leer el mismo total y escribir el mismo resultado, por lo que uno de los incrementos se pierde.

**Respuesta 3 (después del TODO 2):** con el `lock_guard`, ¿qué imprime? ¿Qué reporta ThreadSanitizer antes y después del mutex?

Con `lock_guard` imprime siempre 2000000. Antes del mutex, ThreadSanitizer reporta una data race sobre `total`; despues del mutex no debe reportar esa carrera.

**La versión completa, tal como la resuelvo yo:**

Se crearon `puerta1` y `puerta2` con `std::thread`, y ambos hacen `join()` antes de leer el total. `ContadorDeVisitas` tiene un `std::mutex candado`; `registrar` usa `std::lock_guard<std::mutex> guardia(candado)` antes de sumar. ThreadSanitizer detecta la carrera antes de este cambio y deja de reportarla despues.

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

## Ejercicio 2: la tarjeta del comedor

Archivo: [`ejercicio2_tarjeta_comedor.cpp`](./ejercicio2_tarjeta_comedor.cpp). Solo o en pareja.

Dos cajas cobran de la misma tarjeta prepago al mismo tiempo. El encabezado dice qué tiene que quedar al final.

1. Completa el TODO 1 (los dos hilos), sin mutex todavía. Corre tres veces y anota el saldo final y los cobros.

Sin mutex obtuve estas tres corridas como `saldo aceptados rechazados`: `27842 76116 0`, `9187 92229 0` y `22518 82430 0`. Los valores cambian porque se pierden cambios concurrentes; incluso los contadores no suman siempre los 120000 intentos.

2. Agrega el mutex (TODO 2), pero toma el candado solo alrededor de la resta y de cada contador, dentro de los dos lados del `if`, no alrededor de la pregunta `saldo >= monto`. Corre diez veces. ¿Algún saldo final fue negativo? ¿Por qué puede pasar, si la resta está protegida?

Puede quedar negativo. Dos cajas pueden ver al mismo tiempo que queda un centavo, y luego entrar por turnos al bloque protegido para restarlo. El candado protege cada resta, pero no protege la decision de si habia saldo suficiente.

3. Completa el TODO 3: un solo `lock_guard` al inicio de `cobrar`, que proteja la pregunta y el cambio juntos. Corre diez veces y con ThreadSanitizer.

Con un solo `lock_guard` al inicio de `cobrar`, las corridas dan siempre saldo 0, 100000 aceptados y 20000 rechazados. ThreadSanitizer no debe reportar carreras en la tarjeta.

Responde: `getSaldo()` no toma el candado. ¿Es un problema en este programa? ¿En qué programa sí lo sería?

No es un problema aqui porque se llama despues de los dos `join()`: ya no hay cajas modificando el saldo. Si una pantalla leyera el saldo mientras las cajas cobran, tambien tendria que tomar el candado para no leer al mismo tiempo que una escritura.

## Ejercicio 3: la taquilla del concierto

Archivo: [`ejercicio3_taquilla.cpp`](./ejercicio3_taquilla.cpp). Solo o en pareja.

Cinco vendedores venden los asientos del mismo teatro al mismo tiempo. El encabezado describe la clase `Taquilla`; esta vez no hay código de partida para la clase: la escribes tú. El `main` ya está escrito y verifica las reglas al final. Mientras no escribas la clase, el archivo no compila.

1. Escribe `Taquilla` y corre el programa diez veces: las tres líneas finales tienen que salir siempre iguales a las del encabezado.
2. Córrelo con ThreadSanitizer: no tiene que reportar nada.
3. Responde: ¿qué partes de `Taquilla` son sección crítica? ¿Por qué `reservarPar` no puede llamar dos veces a `reservar`?

Son seccion critica las consultas y cambios de los duenos de asientos, y el contador de vendidos. `reservarPar` debe verificar y reservar los dos asientos bajo un mismo candado; si llamara dos veces a `reservar`, otro vendedor podria quedarse con el segundo asiento entre las dos llamadas y dejaria un par a medias.

4. Responde: un compañero propone poner un solo mutex en `main`, alrededor de los cinco `std::thread`. ¿Funcionaría? ¿Qué se perdería?

No protege el estado de `Taquilla`, porque las operaciones que lo modifican ocurren dentro de los hilos. Si se usara el mismo mutex y `main` lo mantuviera mientras espera los `join()`, los hilos quedarian bloqueados y habria un deadlock. Forzar a los vendedores a trabajar uno por uno tambien perderia la concurrencia.

## Qué construcciones nuevas usa este laboratorio

`#include <thread>` y `std::thread hilo(funcion, argumento1, argumento2)`, con argumentos por valor o como puntero; `join()`. `#include <mutex>`, `std::mutex` como atributo, y `std::lock_guard<std::mutex> guardia(candado);`, que toma el candado al crearse y lo suelta al salir del bloque. Compilar con `-fsanitize=thread`.

## Antes de la próxima sesión

**Lectura:** ["Beyond the Mutex: A Practical Guide to High-Performance Concurrency in C++", Mansi Maheshwari](https://medium.com/@mansi_maheshwari/beyond-the-mutex-a-practical-guide-to-high-performance-concurrency-in-c-a73a0755293e)

Hoy el mutex arregló el contador de visitas. Imagina que, en vez de dos puertas, son 16 hilos y cada uno registra un millón de visitas. Cada `registrar` toma y suelta el candado. ¿Dónde pasa el tiempo el programa? ¿Hace falta un candado completo para sumar 1 a un número?

_(tu respuesta)_
