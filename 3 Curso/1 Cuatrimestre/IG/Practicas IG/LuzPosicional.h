//
// Created by raul on 19/11/20.
//

#ifndef PRACTICAS_IG_LUZPOSICIONAL_H
#define PRACTICAS_IG_LUZPOSICIONAL_H

#include "aux.h"
#include "Luz.h"

class LuzPosicional : public Luz {
public :
    LuzPosicional (Tupla3f posicion ,GLenum idLuzOpenGL, Tupla4f colorAmbiente,Tupla4f colorEspecular,Tupla4f colorDifuso ) ;
    void modificarPosicion(float angulo);
    void animar();

};
#endif //PRACTICAS_IG_LUZPOSICIONAL_H
