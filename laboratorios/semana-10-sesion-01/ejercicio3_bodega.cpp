#include <iostream>
#include <map>
#include <set>
#include <vector>

// Salida esperada:
// Stock: 120=2 210=5 305=10
// Retirar 1 de 120: aceptado
// Retirar 4 de 305: aceptado
// Retirar 1 de 120: aceptado
// Retirar 1 de 120: rechazado
// Retirar 1 de 999: rechazado
// Retirar 5 de 210: aceptado
// Reingreso de 3 unidades de 120
// Retirar 3 de 120: aceptado
// Stock: 120=0 210=0 305=6
// Stock de 999: 0
// Productos registrados: 3
// Retiros: 120 305 120 210 120
// Tercer retiro: 120
// Agotados alguna vez: 120 210

class Bodega {
    private:
        std::map<int, int> stock;
        std::vector<int> historialRetiros;
        std::set<int> agotados;

    public:
        void ingresar(int codigo, int cantidad) {
            stock[codigo] += cantidad;
        }

        bool retirar(int codigo, int cantidad) {
            auto it = stock.find(codigo);
            if (it == stock.end() || it->second < cantidad) {
                return false;
            }

            it->second -= cantidad;
            historialRetiros.push_back(codigo);
            if (it->second == 0) {
                agotados.insert(codigo);
            }
            return true;
        }

        int stockDe(int codigo) {
            auto it = stock.find(codigo);
            if (it == stock.end()) {
                return 0;
            }
            return it->second;
        }

        int cantidadDeProductos() {
            return stock.size();
        }

        int retiroNumero(int posicion) {
            return historialRetiros[posicion];
        }

        void imprimirStock() {
            std::cout << "Stock:";
            for (auto item : stock) {
                std::cout << " " << item.first << "=" << item.second;
            }
            std::cout << std::endl;
        }

        void imprimirRetiros() {
            std::cout << "Retiros:";
            for (int codigo : historialRetiros) {
                std::cout << " " << codigo;
            }
            std::cout << std::endl;
        }

        void imprimirAgotados() {
            std::cout << "Agotados alguna vez:";
            for (int codigo : agotados) {
                std::cout << " " << codigo;
            }
            std::cout << std::endl;
        }
};

void informarRetiro(int codigo, int cantidad, bool aceptado) {
    std::cout << "Retirar " << cantidad << " de " << codigo << ": ";
    if (aceptado) {
        std::cout << "aceptado" << std::endl;
    } else {
        std::cout << "rechazado" << std::endl;
    }
}

int main() {
    Bodega bodega;
    bodega.ingresar(305, 10);
    bodega.ingresar(120, 2);
    bodega.ingresar(210, 5);
    bodega.imprimirStock();

    informarRetiro(120, 1, bodega.retirar(120, 1));
    informarRetiro(305, 4, bodega.retirar(305, 4));
    informarRetiro(120, 1, bodega.retirar(120, 1));
    informarRetiro(120, 1, bodega.retirar(120, 1));
    informarRetiro(999, 1, bodega.retirar(999, 1));
    informarRetiro(210, 5, bodega.retirar(210, 5));

    bodega.ingresar(120, 3);
    std::cout << "Reingreso de 3 unidades de 120" << std::endl;
    informarRetiro(120, 3, bodega.retirar(120, 3));

    bodega.imprimirStock();
    std::cout << "Stock de 999: " << bodega.stockDe(999) << std::endl;
    std::cout << "Productos registrados: " << bodega.cantidadDeProductos() << std::endl;
    bodega.imprimirRetiros();
    std::cout << "Tercer retiro: " << bodega.retiroNumero(2) << std::endl;
    bodega.imprimirAgotados();

    return 0;
}
