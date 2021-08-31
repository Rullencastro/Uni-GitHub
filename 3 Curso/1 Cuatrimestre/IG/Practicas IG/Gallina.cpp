//
// Created by raul on 1/12/20.
//

#include "Gallina.h"

Gallina::Gallina() {
    pierna_izq = new Pierna();
    pierna_dch = new Pierna();
    cabeza = new Cabeza();
    cuerpo = new Cuerpo();
    repliegueCabeza_v = 0.9;repliegueCabeza_h = 0.5;giroCabezaVer = 1.0;giroCabezaHor = 1.0;
    giroCaderaDch = 1.0;giroCaderaIzq= 1.0;giroRodillaDch = 1.0;giroRodillaIzq = 1.0;
    giroTobilloDch = 1.0;giroTobilloIzq = 1.0;giroDedosDch = 1.0;giroDedosIzq = 1.0;
    giroAlas = 1.0;
    fm_alas = 1.01;fm_cadera = 1.01;fm_tobillo = 1.01;fm_cabeza = 1.01;fm_cuello_v = 0.01;fm_dedos = 1.01;fm_rodilla = 1.01;fm_cuello_h = 0.01;
}

void Gallina::draw(unsigned int mode, unsigned int visualizacion, bool tapas) {
    glPushMatrix();
        cuerpo->draw(mode,visualizacion,tapas,giroAlas);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0.6,0.8);
        cabeza->draw(mode,visualizacion,tapas,repliegueCabeza_v,repliegueCabeza_h,giroCabezaVer,giroCabezaHor);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.7,-0.7,0);
        pierna_dch->draw(mode,visualizacion,tapas,giroCaderaDch,giroRodillaDch,giroTobilloDch,giroDedosDch);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.7,-0.7,0);
        pierna_izq->draw(mode,visualizacion,tapas,giroCaderaIzq,giroRodillaIzq,giroTobilloIzq,giroDedosIzq);
    glPopMatrix();
}

void Gallina::modificarGiroCabezaHor(float valor) {
    giroCabezaHor += valor;

    if(giroCabezaHor > 90.0)
        giroCabezaHor = 90.0;
    else if(giroCabezaHor < (-90.0))
        giroCabezaHor = (-90.0);
}
void Gallina::modificarGiroCabezaVer(float valor) {
    giroCabezaVer += valor;

    if(giroCabezaVer > 90.0)
        giroCabezaVer = 90.0;
    else if(giroCabezaVer < (-90.0))
        giroCabezaVer = (-90.0);
}

void Gallina::modificarRepliegueCabeza(float valor_v, float valor_h) {
    repliegueCabeza_v += valor_v;
    repliegueCabeza_h += valor_h;


    if(repliegueCabeza_v > 1.2)
        repliegueCabeza_v = 1.2;
    else if(repliegueCabeza_v < 0.6)
        repliegueCabeza_v = 0.6;

    if(repliegueCabeza_h > 0.7)
        repliegueCabeza_h = 0.7;
    else if(repliegueCabeza_h < 0.3)
        repliegueCabeza_h = 0.3;
}

void Gallina::modificarGiroCaderaDch(float valor) {
    giroCaderaDch += valor;

    if(giroCaderaDch > 45.0)
        giroCaderaDch = 45.0;
    else if(giroCaderaDch < (-45.0))
        giroCaderaDch = (-45.0);
}

void Gallina::modificarGiroCaderaIzq(float valor) {
    giroCaderaIzq += valor;

    if(giroCaderaIzq > 45.0)
        giroCaderaIzq = 45.0;
    else if(giroCaderaIzq < (-45.0))
        giroCaderaIzq = (-45.0);
}

void Gallina::modificarGiroRodillaDch(float valor) {
    giroRodillaDch += valor;

    if(giroRodillaDch > 45.0)
        giroRodillaDch = 45.0;
    else if(giroRodillaDch < 0.0)
        giroRodillaDch = 0.0;
}

void Gallina::modificarGiroRodillaIzq(float valor) {
    giroRodillaIzq += valor;

    if(giroRodillaIzq > 45.0)
        giroRodillaIzq = 45.0;
    else if(giroRodillaIzq < 0.0)
        giroRodillaIzq = 0.0;
}

void Gallina::modificarGiroTobilloDch(float valor) {
    giroTobilloDch += valor;

    if(giroTobilloDch > 30.0)
        giroTobilloDch = 30.0;
    else if(giroTobilloDch < (-30.0))
        giroTobilloDch = (-30.0);
}

void Gallina::modificarGiroTobilloIzq(float valor) {
    giroTobilloIzq += valor;

    if(giroTobilloIzq > 30.0)
        giroTobilloIzq = 30.0;
    else if(giroTobilloIzq < (-30.0))
        giroTobilloIzq = (-30.0);
}

void Gallina::modificarGiroDedosDch(float valor) {
    giroDedosDch += valor;

    if(giroDedosDch > 0.0)
        giroDedosDch = 0.0;
    else if(giroDedosDch < (-45.0))
        giroDedosDch = (-45.0);
}

void Gallina::modificarGiroDedosIzq(float valor) {
    giroDedosIzq += valor;

    if(giroDedosIzq > 0.0)
        giroDedosIzq = 0.0;
    else if(giroDedosIzq < (-45.0))
        giroDedosIzq = (-45.0);
}

void Gallina::modificarGiroAlas(float valor) {
     giroAlas += valor;

    if(giroAlas > 100.0)
        giroAlas = 100.0;
    else if(giroAlas < 0.0)
        giroAlas = 0.0;
}

void Gallina::animacion_manual(unsigned int grado,bool signo){
    float mod_r = 5.0, mod_t = 0.05;
    if(!signo) {
        mod_r = -5.0;
        mod_t = -0.05;
    }
    switch (grado) {
        case 0:
            modificarGiroCabezaVer(mod_r);
            break;
        case 1:
            modificarGiroCabezaHor(mod_r);
            break;
        case 2:
            modificarRepliegueCabeza(mod_t,mod_t);
            break;
        case 3:
            modificarGiroAlas(mod_r);
            break;
        case 4:
            modificarGiroCaderaIzq(mod_r);
            break;
        case 5:
            modificarGiroCaderaDch(mod_r);
            break;
        case 6:
            modificarGiroRodillaIzq(mod_r);
            break;
        case 7:
            modificarGiroRodillaDch(mod_r);
            break;
        case 8:
            modificarGiroTobilloIzq(mod_r);
            break;
        case 9:
            modificarGiroTobilloDch(mod_r);
            break;
        case 10:
            modificarGiroDedosIzq(mod_r);
            break;
        case 11:
            modificarGiroDedosDch(mod_r);
            break;
    }
}

void Gallina::animar() {
    if (giroCabezaHor >= 45.0) {
        fm_cabeza = -fm_cabeza;
    }
    else if (giroCabezaHor <= -45.0 ) {
        fm_cabeza = -fm_cabeza;
    }
    this->modificarGiroCabezaHor(fm_cabeza);
    this->modificarGiroCabezaVer(fm_cabeza);

    if (giroAlas >= 100.0) {
        fm_alas = -fm_alas;
    }
    else if (giroAlas <= std::numeric_limits<float>::epsilon()) {
        fm_alas = -fm_alas;
    }
    this->modificarGiroAlas(fm_alas);

    if (giroCaderaDch >= 45.0) {
        fm_cadera = -fm_cadera;
    }
    else if (giroCaderaDch <= -45.0) {
        fm_cadera = -fm_cadera;
    }
    this->modificarGiroCaderaDch(fm_cadera);
    this->modificarGiroCaderaIzq(-fm_cadera);

    if (giroRodillaDch >= 45.0) {
        fm_rodilla = -fm_rodilla;
    }
    else if (giroRodillaDch <= std::numeric_limits<float>::epsilon()) {
        fm_rodilla = -fm_rodilla;
    }
    this->modificarGiroRodillaDch(fm_rodilla);
    this->modificarGiroRodillaIzq(-fm_rodilla);

    if (giroTobilloDch >= 30.0) {
        fm_tobillo = -fm_tobillo;
    }
    else if (giroTobilloDch <= -30.0) {
        fm_tobillo = -fm_tobillo;
    }
    this->modificarGiroTobilloDch(fm_tobillo);
    this->modificarGiroTobilloIzq(-fm_tobillo);

    if (giroDedosDch >= std::numeric_limits<float>::epsilon()) {
        fm_dedos = -fm_dedos;
    }
    else if (giroDedosIzq <= -45.0) {
        fm_dedos = -fm_dedos;
    }
    this->modificarGiroTobilloDch(fm_dedos);
    this->modificarGiroTobilloIzq(-fm_dedos);

    if (repliegueCabeza_v >= 1.2) {
        fm_cuello_v = -fm_cuello_v;
    }
    else if (repliegueCabeza_v <= 0.7) {
        fm_cuello_v = -fm_cuello_v;
    }
    if (repliegueCabeza_h >= 0.7) {
        fm_cuello_h = -fm_cuello_h;
    }
    else if (repliegueCabeza_h <= 0.3) {
        fm_cuello_h = -fm_cuello_h;
    }

    this->modificarRepliegueCabeza(fm_cuello_v,fm_cuello_h);
}

void Gallina::aumentarVelocidad(unsigned int grado) {
    switch (grado) {
        case 0:
            fm_alas *= 2;
            fm_cadera *= 2;
            fm_rodilla *= 2;
            fm_tobillo *= 2;
            fm_dedos *= 2;
            fm_cuello_v *= 2;
            fm_cuello_h *= 2;
            break;

        case 1:
            fm_alas *= 2;
            break;
        case 2:
            fm_cabeza *= 2;
            break;
        case 3:
            fm_cadera *= 2;
            break;
        case 4:
            fm_rodilla *= 2;
            break;
        case 5:
            fm_tobillo *= 2;
            break;
        case 6:
            fm_dedos *= 2;
            break;
        case 7:
            fm_cuello_v *= 2;
            fm_cuello_h *= 2;
            break;
    }
}

void Gallina::disminuirVelocidad(unsigned int grado) {
    switch (grado) {
        case 0:
            fm_alas /= 2;
            fm_cadera /= 2;
            fm_rodilla /= 2;
            fm_tobillo /= 2;
            fm_dedos /= 2;
            fm_cuello_v /= 2;
            fm_cuello_h /= 2;
            break;

        case 1:
            fm_alas /= 2;
            break;
        case 2:
            fm_cabeza /= 2;
            break;
        case 3:
            fm_cadera /= 2;
            break;
        case 4:
            fm_rodilla /= 2;
            break;
        case 5:
            fm_tobillo /= 2;
            break;
        case 6:
            fm_dedos /= 2;
            break;
        case 7:
            fm_cuello_v /= 2;
            fm_cuello_h /= 2;
            break;
    }
}