//
// Created by raul on 1/12/20.
//

#include "Cabeza.h"
Cabeza::Cabeza() {
    cuello = new Cilindro(5,50,1,1, false);
    cara = new Cara();
    Material plata = Material({0.50754,0.50754,0.50754,1.0},{0.508273,0.508273,0.508273,1.0},{0.19225,0.19225,0.19225,1.0},0.4);
    cuello->setMaterial(plata);
    cuello->cambiarColor({0.8,0.8,0.8});
}
void Cabeza::draw(unsigned int mode, unsigned int visualizacion, bool tapas,float pos_v,float pos_h,float giro_v,float giro_h) {

    glPushMatrix();
        glTranslatef(0,pos_v,pos_h); //y=[1.2,0.6] z=[0.7,0.3]
        cara->draw(mode,visualizacion,tapas,giro_v,giro_h);
    glPopMatrix();

    glPushMatrix();
        glRotatef(30.0,1,0,0);
        glScalef(0.3,1,0.3);
        cuello->draw(mode,visualizacion,tapas);
    glPopMatrix();
}
