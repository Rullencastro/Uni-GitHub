//
// Created by raul on 1/12/20.
//

#ifndef PRACTICAS_IG_CABEZA_H
#define PRACTICAS_IG_CABEZA_H

#include "aux.h"
#include "cilindro.h"
#include "Cara.h"

class Cabeza {
private:
    Cilindro * cuello = nullptr;
    Cara * cara = nullptr;
public:
    Cabeza();
    void draw(unsigned int mode, unsigned int visualizacion, bool tapas,float pos_v,float pos_h,float giro_v,float giro_h);
};


#endif //PRACTICAS_IG_CABEZA_H
