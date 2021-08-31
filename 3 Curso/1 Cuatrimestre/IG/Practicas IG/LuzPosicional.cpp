//
// Created by raul on 19/11/20.
//

#include "LuzPosicional.h"
LuzPosicional::LuzPosicional(Tupla3f posicion ,GLenum idLuzOpenGL, Tupla4f colorAmbiente,Tupla4f colorEspecular,Tupla4f colorDifuso ){
    this->posicion = {posicion(X),posicion(Y),posicion(Z),1.0};
    this->id = idLuzOpenGL;
    this->colorAmbiente = colorAmbiente;
    this->colorEspecular = colorEspecular;
    this->colorDifuso = colorDifuso;
}

void LuzPosicional::modificarPosicion(float angulo) {
    this->posicion(X) = this->posicion(X) * cos(angulo) + this->posicion(Z) * sin(angulo);
    this->posicion(Z) = this->posicion(X) * (-sin(angulo)) + this->posicion(Z) * cos(angulo);
}

void LuzPosicional::animar() {
    modificarPosicion(1.0*M_PI/180);
}