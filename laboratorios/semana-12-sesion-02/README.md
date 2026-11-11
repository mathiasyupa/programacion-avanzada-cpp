# Semana 12, Sesión 2: Build systems (CMake) y debugging/profiling

## Objetivo de la sesión

Configurar un CMakeLists.txt funcional y usar gdb o sanitizers para localizar la causa de un error real en tiempo de ejecución.

## Antes de empezar

**Lectura previa:** ["C++ build systems: our transition from CMake to Meson", Niek J. Bouman](https://niekbouman.medium.com/c-build-systems-our-transition-from-cmake-to-meson-2c043e93822f)

**Referencia:** [CMake Tutorial (documentación oficial)](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

Según el artículo, ¿qué problemas tenía el equipo con CMake que los llevaron a cambiar a Meson? ¿Qué siguió haciendo igual la herramienta nueva?

_(tu respuesta)_

En la sesión pasada compilaste cada archivo de pruebas con un comando largo. Si el curso tuviera 20 archivos y cada uno dependiera de varios `.h`, ¿cómo sabrías qué recompilar cuando cambias uno de ellos? ¿Qué tendría que saber una herramienta para hacerlo por ti?

_(tu respuesta)_

Entra a esta carpeta:

```
cd laboratorios/semana-12-sesion-02
```

CMake, gdb y GoogleTest ya están instalados en el Codespace. Cada ejercicio es un proyecto de CMake en su propia carpeta, y se compila desde esa carpeta:

```
cd ejercicio1
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

La carpeta `build/` la genera CMake y no se edita ni se sube al repositorio.

## Ejercicio 1: el estacionamiento con CMake

Carpeta: [`ejercicio1/`](./ejercicio1/). Tiene el estacionamiento (`estacionamiento.h`), su `main.cpp`, las pruebas de la sesión pasada (`test_estacionamiento.cpp`), un programa nuevo, `reporte_tiempos.cpp`, y un `CMakeLists.txt` con cuatro `TODO`.

**Respuesta 1 (después del TODO 3):** con todo compilado, si corres `cmake --build build` otra vez sin cambiar nada, ¿qué se recompila? ¿Y si cambias solo `main.cpp`?

Si no cambia nada, CMake revisa las dependencias y no recompila ningun archivo. Si cambia solo `main.cpp`, recompila ese archivo y vuelve a enlazar solamente el ejecutable `estacionamiento`.

**Respuesta 2 (después del TODO 3):** si cambias `estacionamiento.h`, ¿qué ejecutables se recompilan? ¿Cómo lo sabe CMake, si ningún `CMakeLists.txt` menciona ese archivo?

Se recompilan `estacionamiento` y `pruebas`, porque ambos incluyen `estacionamiento.h`; `reporte_tiempos` no. CMake usa las dependencias que el compilador genera al compilar los archivos, aunque el encabezado no aparezca escrito en `CMakeLists.txt`.

**Respuesta 3 (después del TODO 4):** `reporte_tiempos` termina con `Segmentation fault`. Antes de abrirlo con gdb: ¿en qué función crees que se detiene, y qué valor tiene el puntero que se desreferencia?

Se detiene en `Estadia::getSalida`. El objeto que intenta usar es nulo: gdb muestra `this=0x0`, porque `ultimaDe(5555)` devolvio `nullptr`.

**La versión completa, tal como la resuelvo yo:**

Se configuraron los tres ejecutables, las pruebas con GoogleTest y `ctest`. Use `gdb -q reporte_tiempos`, despues `run` y `bt`. El backtrace mostro `Estadia::getSalida (this=0x0)`, llamado desde `minutosDe` para la placa 5555. La correccion fue comprobar si `ultimaDe` devuelve `nullptr`; en ese caso `minutosDe` retorna -1 y `main` imprime `sin registro hoy`.

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

## Ejercicio 2: el promedio móvil del invernadero

Carpeta: [`ejercicio2/`](./ejercicio2/). Solo o en pareja.

`lecturas.h` calcula el promedio móvil de las lecturas de un sensor. `main.cpp` imprime la salida esperada de su encabezado, y las dos pruebas de `test_lecturas.cpp` pasan. Aun así, `promedioMovil` tiene un error.

1. Completa los `TODO` 1 a 3 de `CMakeLists.txt`.
2. Compila en `build/`, corre `./build/promedios` y `ctest --test-dir build --output-on-failure`. ¿Coincide la salida con la esperada? ¿Pasan las pruebas?

Sin sanitizers, la salida visible coincide con la esperada y las dos pruebas iniciales pasan. Eso no demuestra que la funcion este bien: las dos ultimas ventanas leen fuera del vector, pero `main` solo imprime los primeros cuatro resultados.

3. Configura una segunda carpeta con sanitizers (el comando está en el encabezado de `CMakeLists.txt`), compila y corre `./build-asan/promedios`. ¿Qué tipo de error reporta AddressSanitizer, en qué archivo y en qué línea? ¿Por qué el programa sin sanitizers no terminó con un error, y por qué su salida era la correcta?

AddressSanitizer reporta un `heap-buffer-overflow` al leer `lecturas[inicio + j]` en `lecturas.h`. Sin sanitizer ese acceso fuera de rango tiene comportamiento indefinido y puede no detener el programa. Los primeros cuatro promedios se calculan con posiciones validas, que son precisamente los que imprime `main`.

4. Escribe la prueba del `TODO 4` de `test_lecturas.cpp` y verifica que falla en `build/` antes de corregir nada.
5. Corrige `lecturas.h`. Las tres pruebas tienen que pasar en `build/` y en `build-asan/`. ¿Qué cambiaste?

Cambie la condicion del ciclo externo a `inicio + ventana <= cantidad`. Asi solo se crean ventanas completas y, con seis lecturas y ventana tres, se generan exactamente cuatro promedios.

Responde: ¿por qué dos carpetas de compilación (`build/` y `build-asan/`) y no una sola que cambia de opción?

Cada carpeta guarda una configuracion y ejecutables distintos. Asi se puede conservar una compilacion normal y otra con sanitizers sin mezclar opciones ni tener que reconfigurar una para usar la otra.

## Ejercicio 3: la cola de impresión de la biblioteca

Carpeta: [`ejercicio3/`](./ejercicio3/). Solo o en pareja.

`cola_impresion.h` describe en su comentario lo que tiene que hacer la cola de impresión, y `main.cpp` trae la salida esperada. El programa tiene dos errores distintos. Esta vez no hay `CMakeLists.txt` ni archivo de pruebas: los escribes tú.

Lo que tiene que quedar en la carpeta:

- Un `CMakeLists.txt` con el ejecutable `cola`, un ejecutable de pruebas con GoogleTest registrado para `ctest`, y la opción `CON_SANITIZERS`.
- Un `test_cola.cpp` con al menos una prueba por error, escrita antes de corregirlo, que falla con el código original.
- `cola_impresion.h` corregido: la salida de `main` coincide con la esperada y todas tus pruebas pasan, con y sin sanitizers.

Completa la tabla con tu diagnóstico de cada error:

| Error | Síntoma | Herramienta que lo localizó | Causa (archivo, línea) | Corrección |
|---|---|---|---|---|
| 1 | 25 paginas daba 2 minutos, no 3 | Prueba unitaria | `cola_impresion.h`, `minutosDeImpresion` usaba division entera | Usar `(paginas + 9) / 10` para redondear hacia arriba |
| 2 | Corrupcion de memoria al dividir trabajos grandes | gdb y sanitizer | `cola_impresion.h`, `push_back` invalidaba el iterador del ciclo | Recorrer con indice y no guardar un iterador mientras se agrega al vector |

Usa gdb al menos una vez, en cualquiera de los dos errores. Pega aquí los comandos que escribiste y lo que respondió gdb:

```
gdb -q cola
(gdb) run
Program received signal SIGABRT
(gdb) bt
... ColaDeImpresion::dividirGrandes ... cola_impresion.h
```

Responde: uno de los dos errores no lo reporta ningún sanitizer. ¿Cuál, y por qué? ¿Qué lo detectó?

El redondeo incorrecto de minutos no lo reporta ningun sanitizer, porque no usa memoria invalida ni produce comportamiento indefinido: es un error de logica. Lo detecto la prueba unitaria de 25 paginas.

## Qué construcciones nuevas usa este laboratorio

`CMakeLists.txt` con `cmake_minimum_required`, `project`, `set(CMAKE_CXX_STANDARD 20)`, `add_executable`, `target_compile_options`, `find_package(GTest REQUIRED)`, `target_link_libraries(... GTest::gtest_main)`, `enable_testing`, `include(GoogleTest)` y `gtest_discover_tests`; `option` e `if` en CMake, con `add_compile_options` y `add_link_options`. Los comandos `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`, `cmake --build build` y `ctest --test-dir build --output-on-failure`. En gdb: `run`, `bt`, `frame N`, `print`, `break` (con condición `if`), `finish`, `quit`. Los sanitizers `-fsanitize=address,undefined`.

## Antes de la próxima sesión

**Lectura:** ["Race Condition: The Invisible Bug That Breaks Your Code", Varbolean](https://medium.com/@varboleansh/race-condition-c5d038870f83)

Hoy cada error apareció siempre igual: misma entrada, mismo `Segmentation fault`, misma línea en el reporte. Imagina un programa que solo falla una de cada mil ejecuciones, con la misma entrada. ¿Qué tendría que estar pasando para que el mismo código, con los mismos datos, dé resultados distintos? ¿Te serviría gdb para encontrarlo?

_(tu respuesta)_
