#include <iostream>

int main() {
    int numero = 10;

    int *puntero_numero = &numero;

    std::cout << "Direccion de memoria: " << puntero_numero << std::endl;
    std::cout << "Valor en memoria: " << *puntero_numero << std::endl;

    int notas[5] = {80, 90, 60, 40, 50};
    int *puntero_notas = notas;
    std::cout << "Direccion de memoria: " << puntero_notas << std::endl;
    std::cout << "Valor en memoria: " << *puntero_notas << std::endl;

    for (int i = 0; i < 5; i++) {
        std::cout << "Valor en memoria: " << *(puntero_notas + i) << std::endl;
    }
    return 0;
}
