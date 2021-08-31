//
// Created by raul on 1/12/20.
//

#include "Cara.h"
Cara::Cara() {
    pico = new Cono(2,50,1,1, false);
    cresta = new Cono(2,50,1,1, false);
    cara = new Esfera(10,50,1, false);
    papada = new Esfera(10,50,1, false);
    ojo = new Semiesfera(10,50,1, false);
    Material oro = Material({0.75164,0.60648,0.22648,1.0},{0.628281,0.555802,0.366065,1.0},{0.24725,0.1995,0.0745,1.0},0.4);
    Material ruby = Material({0.61424,0.04136,0.04136,1.0},{0.727811,0.626959,0.626959,1.0},{0.1745,0.01175,0.01175,1.0},0.6);
    Material plata = Material({0.50754,0.50754,0.50754,1.0},{0.508273,0.508273,0.508273,1.0},{0.19225,0.19225,0.19225,1.0},0.4);
    Material obsidiana = Material({0.18275,0.17,0.22525,1.0},{0.332741,0.328634,0.346435,1.0},{0.05375,0.05,0.06625,1.0},0.3);
    pico->setMaterial(oro);
    pico->cambiarColor({1,1,0});
    cresta->setMaterial(ruby);
    cresta->cambiarColor({1,0,0});
    papada->setMaterial(ruby);
    papada->cambiarColor({1,0,0});
    ojo->setMaterial(obsidiana);
    ojo->cambiarColor({0,0,0});
    cara->setMaterial(plata);
    cara->cambiarColor({0.8,0.8,0.8});
}

void Cara::draw(unsigned int mode, unsigned int visualizacion, bool tapas,float giro_v,float giro_h) {
    glPushMatrix();
        glRotatef(giro_v,1,0,0);//[-90,90]
        glRotatef(giro_h,0,1,0);//[-90,90]

        glPushMatrix();
            glScalef(0.5,0.5,0.5);
            cara->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,-0.1,0.4);
            glRotatef(100.0,1,0,0);
            glScalef(0.2,0.8,0.2);
            pico->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0.2,0.35);
            glRotatef(40.0,1,0,0);
            glScalef(0.15,0.4,0.15);
            cresta->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0.3,0.2);
            glRotatef(20.0,1,0,0);
            glScalef(0.2,0.5,0.2);
            cresta->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0.4,0.0);
            glRotatef(10.0,1,0,0);
            glScalef(0.2,0.6,0.2);
            cresta->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0.35,-0.2);
            glRotatef(-10.0,1,0,0);
            glScalef(0.2,0.6,0.2);
            cresta->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,-0.55,0.15);
            glRotatef(-30.0,1,0,0);
            glScalef(0.1,0.2,0.3);
            papada->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.45,0.1,0.1);
            glRotatef(90.0,0,0,1);
            glScalef(0.1,0.1,0.1);
            ojo->draw(mode,visualizacion,tapas);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.45,0.1,0.1);
            glRotatef(-90.0,0,0,1);
            glScalef(0.1,0.1,0.1);
            ojo->draw(mode,visualizacion,tapas);
        glPopMatrix();
    glPopMatrix();
}
