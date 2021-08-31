//
// Created by raul on 1/12/20.
//

#include "Pierna.h"
Pierna::Pierna() {
    femur = new Cilindro(5,50,1,1,false);
    cadera = new Esfera(10,50,1,false);
    rodilla = new Esfera(10,50,1,false);
    pie = new Pie();
    Material cobre = Material({0.7038,0.27048,0.0828,1.0},{0.256777,0.137622,0.086014,1.0},{0.19125,0.0735,0.0225,1.0},0.1);
    femur->setMaterial(cobre);
    femur->cambiarColor({1,0.5,0});
    cadera->setMaterial(cobre);
    cadera->cambiarColor({1,0.5,0});
    rodilla->setMaterial(cobre);
    rodilla->cambiarColor({1,0.5,0});
}
void Pierna::draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_c,float giro_r,float giro_t,float giro_d) {
    glPushMatrix();
        glRotatef(giro_c,1,0,0);//[-45,45]

        glPushMatrix();
            glScalef(0.1,0.1,0.1);
            cadera->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,-1,0);

            glPushMatrix();
                glTranslatef(0,0.1,0);
                glScalef(0.1,0.8,0.1);
                femur->draw(mode,visualizacion,tapas);
            glPopMatrix();

            glPushMatrix();
                glRotatef(giro_r,1,0,0);//[0,45]

                glPushMatrix();
                    glScalef(0.1,0.1,0.1);
                    rodilla->draw(mode,visualizacion,tapas);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0,-1,0);
                    pie->draw(mode,visualizacion,tapas,giro_t,giro_d);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}