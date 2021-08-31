//
// Created by raul on 30/11/20.
//

#ifndef PRACTICAS_IG_SEMIESFERA_H
#define PRACTICAS_IG_SEMIESFERA_H


#include "aux.h"
#include "objrevolucion.h"

class Semiesfera : public ObjRevolucion {
public:
    Semiesfera(int num_ver_perfil = 10, int num_instancias_per = 20,float r = 1.0 , bool textura = false);

private:
    float radio;
};


#endif //PRACTICAS_IG_SEMIESFERA_H
