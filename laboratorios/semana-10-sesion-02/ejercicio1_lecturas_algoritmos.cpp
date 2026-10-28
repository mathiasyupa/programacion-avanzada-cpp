#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

// Temperaturas de una semana en el laboratorio. Cada calculo esta escrito
// con un loop a mano. Corre el archivo antes de cambiar nada.

int main() {
    std::vector<double> temperaturas = {21.5, 23.0, 19.5, 23.0, 25.5, 22.0, 18.5};

    // TODO (paso 1): reemplaza este loop por std::accumulate, de
    // <numeric>, con 0.0 como valor inicial.
    double suma = std::accumulate(temperaturas.begin(), temperaturas.end(), 0.0);
    std::cout << "Suma: " << suma << std::endl;

    // TODO (paso 2): reemplaza este loop por std::max_element, de
    // <algorithm>. Ojo: devuelve un iterador, no un valor.
    auto maximaIt = std::max_element(temperaturas.begin(), temperaturas.end());
    double maxima = *maximaIt;
    std::cout << "Maxima: " << maxima << std::endl;

    // TODO (paso 3): reemplaza este loop por std::count.
    int vecesEn23 = std::count(temperaturas.begin(), temperaturas.end(), 23.0);
    std::cout << "Dias en 23.0: " << vecesEn23 << std::endl;

    // TODO (paso 4): reemplaza este loop por std::find. Si encuentra
    // 25.5, la posicion es la distancia entre el iterador encontrado y
    // temperaturas.begin().
    auto encontrada = std::find(temperaturas.begin(), temperaturas.end(), 25.5);
    if (encontrada == temperaturas.end()) {
        std::cout << "25.5 no aparece" << std::endl;
    } else {
        int posicion = encontrada - temperaturas.begin();
        std::cout << "25.5 aparece en la posicion " << posicion << std::endl;
    }

    std::list<double> lista(temperaturas.begin(), temperaturas.end());
    double sumaLista = std::accumulate(lista.begin(), lista.end(), 0.0);
    auto maximaListaIt = std::max_element(lista.begin(), lista.end());
    int vecesEn23Lista = std::count(lista.begin(), lista.end(), 23.0);
    std::cout << "Suma (lista): " << sumaLista << std::endl;
    std::cout << "Maxima (lista): " << *maximaListaIt << std::endl;
    std::cout << "Dias en 23.0 (lista): " << vecesEn23Lista << std::endl;

    std::cout << "Suma con 0 como valor inicial: "
              << std::accumulate(temperaturas.begin(), temperaturas.end(), 0)
              << std::endl;

    return 0;
}
