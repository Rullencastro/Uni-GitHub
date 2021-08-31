//
// Created by raul on 15/12/20.
//

#include "Cuadro.h"

Cuadro::Cuadro(float lado)  {
    Tupla3f v1({-lado/2,0,0}), v2({-lado/2,lado,0}), v3({lado/2,0,0}), v4({lado/2,lado,0});
    v={v1,v2,v3,v4};

    Tupla3i t0({0,2,1}), t1({1,2,3});
    f={t0,t1};
    fp={t0};
    fi={t1};

    calcular_normales();

    inicializarColores();
    cambiarColor({1,1,1});
    cambiarColorSel({1,1,1});

    Tupla2f ct1({0,0}) , ct2({0,1}) , ct3({1,0}), ct4({1,1});
    ct = {ct1,ct2,ct3,ct4};

    this->setMaterial(Material({0.0,0.0,0.0,1.0}, {1.0,1.0,1.0,1.0}, {0.0,0.0,0.0,0.0}, 1.0));
}




