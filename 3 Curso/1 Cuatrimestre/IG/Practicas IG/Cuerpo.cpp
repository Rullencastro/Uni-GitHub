//
// Created by raul on 1/12/20.
//

#include "Cuerpo.h"
Cuerpo::Cuerpo() {
    visagra = new Cilindro(5,50,1,1, false);
    tronco = new Esfera(10,50,1, false);
    ala = new Semiesfera(10,50,1, false);
    cola = new Cono(2,50,1,1, false);
    Material plata = Material({0.50754,0.50754,0.50754,1.0},{0.508273,0.508273,0.508273,1.0},{0.19225,0.19225,0.19225,1.0},0.4);
    visagra->setMaterial(plata);
    visagra->cambiarColor({0.8,0.8,0.8});
    tronco->setMaterial(plata);
    tronco->cambiarColor({0.8,0.8,0.8});
    ala->setMaterial(plata);
    ala->cambiarColor({1,1,1});
    cola->setMaterial(plata);
    cola->cambiarColor({0.8,0.8,0.8});
}

void Cuerpo::draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_a) {
    glPushMatrix();
        glRotatef(90,0,0,1);
        glScalef(0.84,1.2,1.44);
        tronco->draw(mode,visualizacion,tapas);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0.2,-1.1);
        glRotatef(-45,1,0,0);
        glScalef(0.4,1,0.4);
        cola->draw(mode,visualizacion,tapas);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-1,0.4,0);
        glRotatef((-giro_a),0,0,1);//[-100,0]

        glPushMatrix();
            glTranslatef(0,0,-0.5);
            glRotatef(90,1,0,0);
            glScalef(0.05,1,0.05);
            visagra->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.2,-0.4,0);
            glRotatef(-115,0,0,1);
            glScalef(0.48,0.24,0.64);
            ala->draw(mode,visualizacion,tapas);
        glPopMatrix();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(1,0.4,0);
        glRotatef(giro_a,0,0,1); //[0,100]

        glPushMatrix();
            glTranslatef(0,0,-0.5);
            glRotatef(90,1,0,0);
            glScalef(0.05,1,0.05);
            visagra->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.2,-0.4,0);
            glRotatef(115,0,0,1);
            glScalef(0.48,0.24,0.64);
            ala->draw(mode,visualizacion,tapas);
        glPopMatrix();
    glPopMatrix();


}