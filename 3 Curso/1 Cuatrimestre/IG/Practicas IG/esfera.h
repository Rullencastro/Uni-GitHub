//
// Created by raul on 5/11/20.
//

#ifndef PRACTICAS_IG_ESFERA_H
#define PRACTICAS_IG_ESFERA_H

#include "aux.h"
#include "objrevolucion.h"

class Esfera : public ObjRevolucion {
public:
    Esfera(int num_ver_perfil = 10, int num_instancias_per = 20,float r = 1.0,bool textura = false);

private:
    float radio;
};

#endif
