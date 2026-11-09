# Semana 12, Sesión 1: Testing unitario (Google Test / Catch2)

## Objetivo de la sesión

Escribir pruebas unitarias con GoogleTest para código propio del curso, y explicar qué garantiza, y qué no, una prueba que pasa.

## Antes de empezar

**Lectura previa:** ["The Illusion of Safety: Why Passing Tests Don't Guarantee Bug-Free Code", Tshegofatsoseane](https://medium.com/@tshegofatsoseane98/the-illusion-of-safety-why-passing-tests-dont-guarantee-bug-free-code-0ce44fd5c43b)

**Referencia:** [GoogleTest Primer](https://google.github.io/googletest/primer.html)

Según el artículo, ¿por qué una suite de pruebas que pasa completa no garantiza que el código no tenga errores? Da un ejemplo de algo que una prueba que pasa no dice.

Que las pruebas solo cubren los casos que fueron escritos. Puede haber una entrada, una combinacion de datos o un requisito que no se probo. Por ejemplo, una suite puede pasar sin verificar que un retiro rechazado no cambie el stock.

En la sesión pasada comparabas a ojo la salida de cada ejercicio con la esperada. ¿Qué tendría que hacer un programa para compararla por ti cada vez que cambias el código? ¿Qué no podría detectar aunque todas las líneas coincidan?

Tendria que ejecutar el programa, guardar su salida y compararla automaticamente con un texto esperado. Aunque las lineas coincidan, no detectaria problemas internos como una fuga de memoria, un dato equivocado que no se imprime o un caso de entrada que no se ejecuto.

Entra a esta carpeta y crea la carpeta de los ejecutables:

```
cd laboratorios/semana-12-sesion-01
mkdir -p bin
```

GoogleTest ya está instalado en el Codespace (paquete `libgtest-dev`). Cada archivo de pruebas se compila con dos bibliotecas más que un programa normal, en este orden:

```
g++ -std=c++20 -Wall -Wextra -g ejercicio1_test_estacionamiento.cpp -lgtest_main -lgtest -pthread -o bin/test_estacionamiento
./bin/test_estacionamiento
```

`-lgtest_main` trae un `main` que corre todas las pruebas del archivo: tu archivo de pruebas no escribe `main`.

## Ejercicio 1: pruebas para el estacionamiento

Archivos: [`estacionamiento.h`](./estacionamiento.h), el estacionamiento de la sesión pasada en su propio archivo, y [`ejercicio1_test_estacionamiento.cpp`](./ejercicio1_test_estacionamiento.cpp), con una prueba escrita y tres `TODO`. Compílalo y córrelo una vez antes de cambiar nada.

**Respuesta 1 (antes del TODO 1):** si en la primera prueba cambias `EXPECT_EQ(*puesto, 0);` por `EXPECT_EQ(*puesto, 1);`, ¿qué crees que imprime GoogleTest? ¿Termina el programa en ese punto o sigue con las demás pruebas?

GoogleTest marca una falla: indica que esperaba 1 y recibio 0. Como se usa `EXPECT_EQ`, la prueba continua despues de esa comprobacion y el programa tambien sigue con las demas pruebas.

**Respuesta 2 (después del TODO 3):** con las cuatro pruebas pasando, borra la línea `ocupado[*puesto] = false;` de `liberar` en `estacionamiento.h`. ¿Cuáles de las cuatro pruebas van a fallar? ¿Por qué `ASSERT_TRUE` y no `EXPECT_TRUE` antes de `*nueva`?

Falla `PuestoLiberadoSeReusa`, porque el puesto 1 queda marcado como ocupado y la siguiente placa no recibe ese puesto. Las otras tres pruebas pasan. Se usa `ASSERT_TRUE` para detener esa prueba si `nueva` esta vacio: asi no se intenta hacer `*nueva` sobre un `optional` vacio.

**Respuesta 3 (con la línea restaurada):** las cuatro pruebas pasan. Escribe un cambio en `estacionamiento.h` que haga que el estacionamiento funcione mal y que ninguna de las cuatro pruebas detecte.

Se puede borrar `puestoDePlaca.erase(placa);` de `liberar`. Liberar una placa y luego intentar asignar la misma placa funcionaria mal, pero ninguna de estas cuatro pruebas intenta asignar de nuevo la placa liberada.

**La versión completa, tal como la resuelvo yo:**

Pruebas nuevas: `LlenoDevuelveVacio`, `PlacaRepetidaLanza` y `PuestoLiberadoSeReusa`. Al borrar `ocupado[*puesto] = false;`, falla `PuestoLiberadoSeReusa`; al restaurarla, las cuatro pruebas terminan con `PASSED`.

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

## Ejercicio 2: la bodega reordenada

Archivos: [`bodega.h`](./bodega.h) y [`ejercicio2_test_bodega.cpp`](./ejercicio2_test_bodega.cpp). Solo o en pareja.

La bodega de la Semana 11, Sesión 1, fue reordenada por alguien del equipo y nadie volvió a correr el `main` de esa sesión. El comentario de `bodega.h` dice lo que tiene que cumplir. Escribe las cinco pruebas de los `TODO`, una por una, compilando y corriendo después de cada una. No cambies `bodega.h` hasta que una prueba falle.

Cuando una prueba falle, copia aquí lo que imprime GoogleTest y explica qué dice cada línea:

Antes de la correccion, GoogleTest informa que `Bodega.CodigoDesconocidoLanzaYNoSeCrea` falla: esperaba que `bodega.stockDe(999)` lanzara `std::out_of_range`, pero no lanzo nada. La linea de fallo identifica la comprobacion; el resumen final indica una prueba fallida de cinco.

Corrige `bodega.h` para que las cinco pruebas pasen. ¿Qué cambiaste, y por qué la versión reordenada creaba el código 999?

Primero se verifico si el codigo existe y solo despues se consulto su stock. Antes se usaba `stock[codigo]` antes de `stock.count(codigo)`: el operador `[]` crea el codigo con stock 0 cuando no existe, por eso aparecia el codigo 999 aunque el retiro fuera rechazado.

Responde: si la prueba del TODO 5 fuera `EXPECT_THROW(bodega.retirar(999, 1), std::exception);`, sin la segunda verificación, ¿habría detectado el error? ¿Qué te dice eso sobre una prueba que pasa?

No. El retiro si lanzaba `std::out_of_range`, asi que esa unica comprobacion pasaba. Sin verificar despues `stockDe(999)`, la prueba no detectaba el efecto incorrecto de haber creado el codigo. Una prueba que pasa solo demuestra lo que comprobo.

## Ejercicio 3: cuatro tarifas, una correcta

Carpeta: [`ejercicio3/`](./ejercicio3/). Solo o en pareja.

`ejercicio3/test_tarifa.cpp` describe la tarifa del estacionamiento de visitas de la universidad. Las carpetas `version_a` a `version_d` tienen cuatro implementaciones de `tarifaEnCentavos`, y solo una cumple la descripción. Escribe tus pruebas a partir de la descripción, antes de abrir cualquier `tarifa.h`, y compila el mismo archivo de pruebas contra cada versión (el comando está en el encabezado). Tu suite tiene que fallar en las versiones con errores y pasar completa en la correcta.

Completa la tabla con el resultado de tu suite:

| Versión | Pruebas que fallan | ¿Cumple la descripción? |
|---|---|---|
| `version_a` | `HastaQuinceMinutosEsGratis` | No |
| `version_b` | `CobraPorHoraOFraccion` | No |
| `version_c` | Ninguna | Si |
| `version_d` | `MinutosNegativosLanzan` | No |

Responde: ¿qué valores de `minutos` elegiste para tus pruebas, y por qué esos y no otros?

Use -1 para el caso invalido; 0 y 15 para el limite gratis; 16, 60 y 61 para el inicio, el final y el siguiente minuto de una hora; y 601 para comprobar el maximo. Son limites donde un error de comparacion o de redondeo se nota.

Responde: la versión que pasa tu suite completa, ¿está demostrada correcta? Describe un error que una quinta versión podría tener y que tu suite no detectaría.

No queda demostrada correcta: solo pasa los casos elegidos. Una quinta version podria cobrar 150 centavos exactamente a los 120 minutos y esta suite no lo detectaria; por eso se necesitan mas casos, sobre todo en limites.

## Qué construcciones nuevas usa este laboratorio

`#include "archivo.h"` con comillas para incluir código propio, y `#pragma once` al inicio de ese archivo; `#include <gtest/gtest.h>`; `TEST(Grupo, NombreDeLaPrueba) { ... }`; `EXPECT_EQ`, `EXPECT_TRUE`, `EXPECT_FALSE`, `EXPECT_THROW(sentencia, tipo)` y `EXPECT_NO_THROW(sentencia)`; `ASSERT_TRUE` y la diferencia entre `ASSERT_` (detiene la prueba) y `EXPECT_` (la deja seguir); compilar con `-lgtest_main -lgtest -pthread`.

## Antes de la próxima sesión

**Lectura:** ["C++ build systems: our transition from CMake to Meson", Niek J. Bouman](https://niekbouman.medium.com/c-build-systems-our-transition-from-cmake-to-meson-2c043e93822f)

Hoy compilaste cada archivo de pruebas con un comando largo, y el Ejercicio 3 lo repetiste cuatro veces cambiando una carpeta. Si el curso tuviera 20 archivos y cada uno dependiera de varios `.h`, ¿cómo sabrías qué recompilar cuando cambias uno de ellos? ¿Qué tendría que saber una herramienta para hacerlo por ti?

La herramienta tendria que conocer los archivos fuente, los encabezados de los que depende cada uno y los ejecutables que debe crear. Con esa relacion puede recompilar solo los objetivos afectados cuando cambia un `.cpp` o un `.h`.
