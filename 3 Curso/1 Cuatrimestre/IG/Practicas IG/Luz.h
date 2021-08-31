//
// Created by raul on 19/11/20.
//

#ifndef PRACTICAS_IG_LUZ_H
#define PRACTICAS_IG_LUZ_H

#include "aux.h"

class Luz {
protected:
    Tupla4f posicion,colorAmbiente,colorDifuso,colorEspecular;
    GLenum id;

public:
    void activar();
    void desactivar();
};

#endif //PRACTICAS_IG_LUZ_H
