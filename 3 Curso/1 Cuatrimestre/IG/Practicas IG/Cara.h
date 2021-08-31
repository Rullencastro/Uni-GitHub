//
// Created by raul on 1/12/20.
//

#ifndef PRACTICAS_IG_CARA_H
#define PRACTICAS_IG_CARA_H

#include "aux.h"
#include "esfera.h"
#include "cono.h"
#include "semiesfera.h"

class Cara {
private:
    Cono * pico = nullptr;
    Cono * cresta = nullptr;
    Esfera * cara = nullptr;
    Esfera * papada = nullptr;
    Semiesfera * ojo = nullptr;
public:
    Cara();
    void draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_v,float giro_h);
};


#endif //PRACTICAS_IG_CARA_H
