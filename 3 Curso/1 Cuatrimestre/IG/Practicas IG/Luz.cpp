//
// Created by raul on 19/11/20.
//

#include "Luz.h"

void Luz::activar() {
    glLightfv(id,GL_POSITION,posicion);
    glLightfv(id,GL_AMBIENT,colorAmbiente);
    glLightfv(id,GL_SPECULAR,colorEspecular);
    glLightfv(id,GL_DIFFUSE,colorDifuso);
    glEnable(id);
}
void Luz::desactivar(){
    glDisable(id);
}