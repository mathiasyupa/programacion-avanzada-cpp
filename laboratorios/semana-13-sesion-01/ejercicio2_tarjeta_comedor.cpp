#include <iostream>
#include <mutex>
#include <thread>

// Ejercicio 2: la tarjeta prepago del comedor, cobrada desde dos cajas.
//
// La tarjeta de un grupo de estudiantes tiene 100000 centavos. Dos cajas
// intentan cobrar 1 centavo, 60000 veces cada una, al mismo tiempo. Solo
// alcanzan 100000 cobros: los demas tienen que rechazarse. Al final, la
// tarjeta tiene que quedar en 0, nunca en negativo, y aceptados mas
// rechazados tiene que dar 120000.
//
// Compilar:  g++ -std=c++20 -Wall -Wextra -g ejercicio2_tarjeta_comedor.cpp -o bin/ejercicio2
// Ejecutar:  ./bin/ejercicio2
//
// Salida esperada (cuando la tarjeta es correcta):
// Saldo final: 0
// Cobros aceptados: 100000
// Cobros rechazados: 20000

class TarjetaDelComedor {
    private:
        int saldo;
        int aceptados;
        int rechazados;
        std::mutex candado;
    public:
        TarjetaDelComedor(int inicial) {
            saldo = inicial;
            aceptados = 0;
            rechazados = 0;
        }

        // Cobra monto si alcanza el saldo, y cuenta el cobro como aceptado
        // o rechazado.
        void cobrar(int monto) {
            std::lock_guard<std::mutex> guardia(candado);
            if (saldo >= monto) {
                saldo = saldo - monto;
                aceptados = aceptados + 1;
            } else {
                rechazados = rechazados + 1;
            }
        }

        int getSaldo() {
            return saldo;
        }

        int getAceptados() {
            return aceptados;
        }

        int getRechazados() {
            return rechazados;
        }
};

void caja(TarjetaDelComedor* tarjeta, int intentos) {
    for (int i = 0; i < intentos; i++) {
        tarjeta->cobrar(1);
    }
}

int main() {
    TarjetaDelComedor tarjeta(100000);

    std::thread caja1(caja, &tarjeta, 60000);
    std::thread caja2(caja, &tarjeta, 60000);
    caja1.join();
    caja2.join();

    std::cout << "Saldo final: " << tarjeta.getSaldo() << std::endl;
    std::cout << "Cobros aceptados: " << tarjeta.getAceptados() << std::endl;
    std::cout << "Cobros rechazados: " << tarjeta.getRechazados() << std::endl;
    return 0;
}
