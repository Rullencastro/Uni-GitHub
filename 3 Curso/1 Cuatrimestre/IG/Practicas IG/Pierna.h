//
// Created by raul on 1/12/20.
//

#ifndef PRACTICAS_IG_PIERNA_H
#define PRACTICAS_IG_PIERNA_H

#include "aux.h"
#include "esfera.h"
#include "cilindro.h"
#include "Pie.h"
class Pierna {
private:
    Cilindro * femur = nullptr;
    Esfera * cadera = nullptr;
    Esfera * rodilla = nullptr;
    Pie * pie = nullptr;
public:
    Pierna();
    void draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_c,float giro_r,float giro_t,float giro_d);
};


#endif //PRACTICAS_IG_PIERNA_H
