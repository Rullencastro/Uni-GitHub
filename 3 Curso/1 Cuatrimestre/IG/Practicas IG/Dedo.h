//
// Created by raul on 1/12/20.
//

#ifndef PRACTICAS_IG_DEDO_H
#define PRACTICAS_IG_DEDO_H

#include "aux.h"
#include "cilindro.h"
#include "cono.h"

class Dedo {
private:
    Cilindro * falange = nullptr;
    Cono * pezunia = nullptr;
public:
    Dedo();
    void draw(unsigned int mode, unsigned int visualizacion, bool tapas);
};


#endif //PRACTICAS_IG_DEDO_H
