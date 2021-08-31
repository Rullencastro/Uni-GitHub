//
// Created by raul on 1/12/20.
//

#include "Dedo.h"

Dedo::Dedo() {
    falange = new Cilindro(5,50,1,1,false);
    pezunia = new Cono(2,50,1,1,false);
    Material cobre = Material({0.7038,0.27048,0.0828,1.0},{0.256777,0.137622,0.086014,1.0},{0.19125,0.0735,0.0225,1.0},0.1);
    falange->setMaterial(cobre);
    falange->cambiarColor({1,0.5,0});
    pezunia->setMaterial(cobre);
    pezunia->cambiarColor({1,0.5,0});
}
void Dedo::draw(unsigned int mode, unsigned int visualizacion, bool tapas) {
    glPushMatrix();
        glRotatef(90,1,0,0);
        glPushMatrix();
            glScalef(0.1,0.6,0.1);
            falange->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0.6,0);
            glScalef(0.1,0.3,0.1);
            pezunia->draw(mode,visualizacion,tapas);
        glPopMatrix();
    glPopMatrix();
}