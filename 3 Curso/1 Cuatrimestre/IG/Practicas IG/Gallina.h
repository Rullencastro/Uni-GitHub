//
// Created by raul on 1/12/20.
//

#ifndef PRACTICAS_IG_GALLINA_H
#define PRACTICAS_IG_GALLINA_H

#include "aux.h"
#include "Cabeza.h"
#include "Pierna.h"
#include "Cuerpo.h"
class Gallina {
    Pierna * pierna_izq = nullptr;
    Pierna * pierna_dch = nullptr;
    Cabeza * cabeza = nullptr;
    Cuerpo * cuerpo = nullptr;
    float giroCabezaVer,giroCabezaHor,repliegueCabeza_v,repliegueCabeza_h,giroAlas;
    float giroRodillaDch,giroRodillaIzq,giroCaderaDch,giroCaderaIzq,giroTobilloDch,giroTobilloIzq,giroDedosDch,giroDedosIzq;
    float fm_alas,fm_rodilla,fm_cuello_v,fm_cuello_h,fm_cabeza,fm_cadera,fm_tobillo,fm_dedos;
public:
    Gallina();
    void draw(unsigned int mode, unsigned int visualizacion, bool tapas);
    void modificarGiroCabezaHor(float valor);
    void modificarGiroCabezaVer(float valor);
    void modificarRepliegueCabeza(float valor_v, float valor_h);
    void modificarGiroRodillaDch(float valor);
    void modificarGiroRodillaIzq(float valor);
    void modificarGiroTobilloDch(float valor);
    void modificarGiroTobilloIzq(float valor);
    void modificarGiroCaderaDch(float valor);
    void modificarGiroCaderaIzq(float valor);
    void modificarGiroDedosDch(float valor);
    void modificarGiroDedosIzq(float valor);
    void modificarGiroAlas(float valor);
    void animacion_manual(unsigned int grado,bool signo);
    void animar();
    void aumentarVelocidad(unsigned int grado);
    void disminuirVelocidad(unsigned int grado);
};


#endif //PRACTICAS_IG_GALLINA_H
