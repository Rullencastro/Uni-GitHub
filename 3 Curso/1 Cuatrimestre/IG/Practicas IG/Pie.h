//
// Created by raul on 1/12/20.
//

#ifndef PRACTICAS_IG_PIE_H
#define PRACTICAS_IG_PIE_H

#include "aux.h"
#include "cilindro.h"
#include "esfera.h"
#include "Dedo.h"
class Pie {
private:
    Cilindro * espinilla = nullptr;
    Esfera * tobillo = nullptr;
    Dedo * dedo = nullptr;
public:
    Pie();
    void draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_t ,float giro_d);
};


#endif //PRACTICAS_IG_PIE_H
