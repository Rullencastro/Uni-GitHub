//
// Created by raul on 5/11/20.
//

#ifndef PRACTICAS_IG_CONO_H
#define PRACTICAS_IG_CONO_H

#include "aux.h"
#include "objrevolucion.h"

class Cono : public ObjRevolucion {
public:
    Cono(int num_ver_perfil = 2, int num_instancias_per = 20, float h = 1.0, float r = 1.0, bool textura = false);

private:
    float altura, radio;
};



#endif
