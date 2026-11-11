# Configuracion de CMake del estacionamiento. Hoy la completamos en clase.
#
# Configurar (una vez, o cuando cambia este archivo):
#     cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# Compilar (cada vez que cambias el codigo):
#     cmake --build build
# Correr las pruebas:
#     ctest --test-dir build --output-on-failure


cmake_minimum_required(VERSION 3.20)
project(Estacionamiento LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(estacionamiento main.cpp)
target_compile_options(estacionamiento PRIVATE -Wall -Wextra)

find_package(GTest REQUIRED)

add_executable(pruebas test_estacionamiento.cpp)
target_compile_options(pruebas PRIVATE -Wall -Wextra)
target_link_libraries(pruebas PRIVATE GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(pruebas)

add_executable(reporte_tiempos reporte_tiempos.cpp)
target_compile_options(reporte_tiempos PRIVATE -Wall -Wextra)
