//
// Created by raul on 19/11/20.
//

#include "LuzDireccional.h"

LuzDireccional::LuzDireccional(Tupla2f direccion, GLenum idLuzOpenGL, Tupla4f colorAmbiente, Tupla4f colorEspecular, Tupla4f colorDifuso) {
    this->alpha =direccion(X)*PI/180;
    this->beta = direccion(Y)*PI/180;

    this->id = idLuzOpenGL;
    this->colorAmbiente = colorAmbiente;
    this->colorEspecular = colorEspecular;
    this->colorDifuso = colorDifuso;

    this->posicion = {(float)(1.0*sin(alpha)*cos(beta)),(float)(1.0*sin(alpha)*sin(beta)),(float)(1.0*cos(alpha)),0.0};
}
void LuzDireccional::variarAnguloAlpha(float incremento) {
    this->alpha += (incremento * PI / 180);
    if (this->alpha > (2 * PI))
        this->alpha -= (2 * PI);
    else
        this->alpha += (2 * PI);
    this->posicion = {(float) (1.0 * sin(alpha) * cos(beta)), (float) (1.0 * sin(alpha) * sin(beta)),(float) (1.0 * cos(alpha)), 0.0};
}
void LuzDireccional::variarAnguloBeta(float incremento) {
    this->beta += (incremento*PI/180);
    if(this->beta > (2*PI))
        this->beta -= (2*PI);
    else
        this->beta += (2*PI);
    this->posicion = {(float)(1.0*sin(alpha)*cos(beta)),(float)(1.0*sin(alpha)*sin(beta)),(float)(1.0*cos(alpha)),0.0};
}