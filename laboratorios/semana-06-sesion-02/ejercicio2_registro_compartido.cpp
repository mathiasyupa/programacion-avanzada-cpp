#include <iostream>
#include <memory>
#include <utility>

class RegistroDeVuelo {
    private:
        std::unique_ptr<double[]> alturas;
        int capacidad;

    public:
        RegistroDeVuelo(int nuevaCapacidad) {
            capacidad = nuevaCapacidad;
            alturas = std::make_unique<double[]>(capacidad);
            std::cout << "Registro de vuelo creado para " << capacidad << " lecturas" << std::endl;
        }

        void guardarAltura(int indice, double valor) {
            alturas[indice] = valor;
        }

        double getAltura(int indice) {
            return alturas[indice];
        }
};

class MonitorDeVuelo {
    private:
        std::shared_ptr<RegistroDeVuelo> registro;
        int idMonitor;

    public:
        MonitorDeVuelo(std::shared_ptr<RegistroDeVuelo> unRegistro, int unId) {
            registro = std::move(unRegistro);
            idMonitor = unId;
        }

        void reportar(int indice) {
            std::cout << "Monitor " << idMonitor << " ve altura "
                      << registro->getAltura(indice) << std::endl;
        }

        ~MonitorDeVuelo() {
            std::cout << "Monitor " << idMonitor << " desconectado" << std::endl;
        }
};

int main() {
    RegistroDeVuelo registroSolo(3);
    registroSolo.guardarAltura(0, 100.0);
    std::cout << "registroSolo.getAltura(0) = " << registroSolo.getAltura(0) << std::endl;

    std::cout << "---" << std::endl;

    std::shared_ptr<RegistroDeVuelo> compartido = std::make_shared<RegistroDeVuelo>(5);
    compartido->guardarAltura(0, 250.0);

    std::cout << "use_count antes de crear monitores: "
              << compartido.use_count() << std::endl;

    {
        MonitorDeVuelo torre(compartido, 1);
        MonitorDeVuelo cabina(compartido, 2);

        std::cout << "use_count con 2 monitores activos: "
                  << compartido.use_count() << std::endl;

        torre.reportar(0);
        cabina.reportar(0);
    }

    std::cout << "use_count despues de destruir monitores: "
              << compartido.use_count() << std::endl;

    return 0;
}
