#include <gtest/gtest.h>

#include "cola_impresion.h"

TEST(ColaDeImpresion, BloqueIncompletoCuentaUnMinutoEntero) {
    EXPECT_EQ(minutosDeImpresion(25), 3);
    EXPECT_EQ(minutosDeImpresion(20), 2);
}

TEST(ColaDeImpresion, DivideTrabajosGrandesHastaVeintePaginas) {
    ColaDeImpresion cola;
    cola.agregar(45);

    cola.dividirGrandes();

    ASSERT_EQ(cola.cantidad(), 3);
    EXPECT_EQ(cola.paginasDe(0), 20);
    EXPECT_EQ(cola.paginasDe(1), 20);
    EXPECT_EQ(cola.paginasDe(2), 5);
}
