//
// Created by raul on 19/11/20.
//

#ifndef PRACTICAS_IG_LUZDIRECCIONAL_H
#define PRACTICAS_IG_LUZDIRECCIONAL_H

#include "aux.h"
#include "Luz.h"

class LuzDireccional : public Luz{
protected:
    float alpha,beta;
    const float PI = 3.141592653589793238462;
public:
    LuzDireccional (Tupla2f direccion ,GLenum idLuzOpenGL, Tupla4f colorAmbiente,Tupla4f colorEspecular,Tupla4f colorDifuso ) ;

    void variarAnguloAlpha( float incremento);
    void variarAnguloBeta( float incremento);
};

#endif //PRACTICAS_IG_LUZDIRECCIONAL_H
