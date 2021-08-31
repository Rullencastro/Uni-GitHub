//
// Created by raul on 1/12/20.
//

#include "Pie.h"


Pie::Pie() {
    espinilla = new Cilindro(5,50,1,1, false);
    tobillo = new Esfera(10,50,1, false);
    dedo = new Dedo();
    Material cobre = Material({0.7038,0.27048,0.0828,1.0},{0.256777,0.137622,0.086014,1.0},{0.19125,0.0735,0.0225,1.0},0.1);
    espinilla->setMaterial(cobre);
    espinilla->cambiarColor({1,0.5,0});
    tobillo->setMaterial(cobre);
    tobillo->cambiarColor({1,0.5,0});
}
void Pie::draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_t ,float giro_d) {

    glPushMatrix();
        glTranslatef(0, 0.1, 0);
        glScalef(0.1, 0.8, 0.1);
        espinilla->draw(mode, visualizacion, tapas);
    glPopMatrix();

    glPushMatrix();
        glRotatef(giro_t,0,1,0);//[-30,30]
        glPushMatrix();
            glScalef(0.1, 0.1, 0.1);
            tobillo->draw(mode, visualizacion, tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0,0.1);
            glRotatef(giro_d,1,0,0);//[-45,0]
            glPushMatrix();
                glRotatef(45,0,1,0);
                dedo->draw(mode, visualizacion, tapas);
            glPopMatrix();

            glPushMatrix();
                glRotatef(-45,0,1,0);
                dedo->draw(mode, visualizacion, tapas);
            glPopMatrix();

            glPushMatrix();
                dedo->draw(mode, visualizacion, tapas);
            glPopMatrix();

        glPopMatrix();
    glPopMatrix();
}