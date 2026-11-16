#include <iostream>
#include <mutex>
#include <thread>

// Ejercicio 1: el contador de visitas de la biblioteca, desde dos puertas.
//
// Cada puerta tiene un sensor que llama registrar() una vez por persona.
// Las dos puertas trabajan al mismo tiempo, cada una en su propio hilo.
// Hoy entran 1000000 personas por cada puerta.
//
// Compilar:  g++ -std=c++20 -Wall -Wextra -g ejercicio1_contador_visitas.cpp -o bin/ejercicio1
// Ejecutar:  ./bin/ejercicio1
// Con ThreadSanitizer:
//   g++ -std=c++20 -Wall -Wextra -g -fsanitize=thread ejercicio1_contador_visitas.cpp -o bin/ejercicio1_tsan
//
// Salida esperada (cuando el contador es correcto):
// Visitas registradas: 2000000

class ContadorDeVisitas {
    private:
        int total;
        std::mutex candado;
    public:
        ContadorDeVisitas() {
            total = 0;
        }

        void registrar() {
            std::lock_guard<std::mutex> guardia(candado);
            total = total + 1;
        }

        int getTotal() {
            return total;
        }
};

void puerta(ContadorDeVisitas* contador, int personas) {
    for (int i = 0; i < personas; i++) {
        contador->registrar();
    }
}

int main() {
    ContadorDeVisitas contador;

    std::thread puerta1(puerta, &contador, 1000000);
    std::thread puerta2(puerta, &contador, 1000000);
    puerta1.join();
    puerta2.join();

    std::cout << "Visitas registradas: " << contador.getTotal() << std::endl;
    return 0;
}
