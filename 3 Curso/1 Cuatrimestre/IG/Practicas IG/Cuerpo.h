//
// Created by raul on 1/12/20.
//

#ifndef PRACTICAS_IG_CUERPO_H
#define PRACTICAS_IG_CUERPO_H

#include "aux.h"
#include "esfera.h"
#include "semiesfera.h"
#include "cilindro.h"
#include "cono.h"

class Cuerpo {
private:
    Cilindro * visagra = nullptr;
    Esfera * tronco = nullptr;
    Semiesfera * ala = nullptr;
    Cono * cola = nullptr;
public:
    Cuerpo();
    void draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_a);
};


#endif //PRACTICAS_IG_CUERPO_H
