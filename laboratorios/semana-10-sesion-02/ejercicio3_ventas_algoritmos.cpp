#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Salida esperada:
// Total recaudado: 146.5
// Ventas de al menos 20: 3
// Primera venta de 210: 4 unidades
// No hay ventas de 999
// Ordenadas por total: 120 (56) 410 (36) 120 (28) 210 (14) 305 (10) 305 (2.5)

// Las ventas de la tienda del laboratorio de electronica. Resuelve cada
// TODO con un algoritmo de la STL, sin escribir ningun loop propio salvo
// el que ya imprime las ventas ordenadas. Incluye arriba los encabezados
// que necesites.

class Venta {
    private:
        int codigo;
        int cantidad;
        double precioUnitario;
    public:
        Venta(int nuevoCodigo, int nuevaCantidad, double nuevoPrecio) {
            codigo = nuevoCodigo;
            cantidad = nuevaCantidad;
            precioUnitario = nuevoPrecio;
        }

        int getCodigo() {
            return codigo;
        }

        int getCantidad() {
            return cantidad;
        }

        double getTotal() {
            return cantidad * precioUnitario;
        }
};

int main() {
    std::vector<Venta> ventas = {
        Venta(305, 10, 0.25),
        Venta(120, 2, 28.0),
        Venta(210, 4, 3.5),
        Venta(120, 1, 28.0),
        Venta(410, 3, 12.0),
        Venta(305, 40, 0.25),
    };
    double umbral = 20.0;

    double totalRecaudado = std::accumulate(
        ventas.begin(), ventas.end(), 0.0, [](double acumulado, Venta venta) {
            return acumulado + venta.getTotal();
        }
    );
    std::cout << "Total recaudado: " << totalRecaudado << std::endl;

    int ventasGrandes = std::count_if(
        ventas.begin(), ventas.end(), [umbral](Venta venta) {
            return venta.getTotal() >= umbral;
        }
    );
    std::cout << "Ventas de al menos 20: " << ventasGrandes << std::endl;

    int codigoBuscado = 210;
    auto venta210 = std::find_if(
        ventas.begin(), ventas.end(), [codigoBuscado](Venta venta) {
            return venta.getCodigo() == codigoBuscado;
        }
    );
    std::cout << "Primera venta de 210: " << venta210->getCantidad()
              << " unidades" << std::endl;

    codigoBuscado = 999;
    auto venta999 = std::find_if(
        ventas.begin(), ventas.end(), [codigoBuscado](Venta venta) {
            return venta.getCodigo() == codigoBuscado;
        }
    );
    if (venta999 == ventas.end()) {
        std::cout << "No hay ventas de 999" << std::endl;
    }

    std::sort(ventas.begin(), ventas.end(), [](Venta a, Venta b) {
        return a.getTotal() > b.getTotal();
    });

    std::cout << "Ordenadas por total:";
    for (Venta v : ventas) {
        std::cout << " " << v.getCodigo() << " (" << v.getTotal() << ")";
    }
    std::cout << std::endl;

    return 0;
}
