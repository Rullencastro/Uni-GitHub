//
// Created by raul on 23/12/20.
//

#ifndef PRACTICAS_IG_CAMARA_H
#define PRACTICAS_IG_CAMARA_H

#include "aux.h"

class Camara {
private:
    Tupla3f eye ,eyeDefecto;
    Tupla3f at , atDefecto;
    Tupla3f up ;
    int tipo ; // ORTOGONAL o Perspectiva
    float left , right , near , far,top,bottom; // o bien aspect, fov, near, far;
    bool objetoSeleccionado;
public:
    Camara (Tupla3f eye, Tupla3f at, Tupla3f up, int tipo, float left, float right, float near, float far, float top, float bottom) ; // con los parametros necesarios
    void rotarXExaminar ( float angle );
    void rotarYExaminar ( float angle );
    void rotarZExaminar ( float angle );
    void rotarXFirstPerson ( float angle );
    void rotarYFirstPerson ( float angle );
    void rotarZFirstPerson ( float angle );
    void mover ( float x, float y, float z );
    void zoom ( float factor );
    void setObserver ();
    void setProyeccion ();
    void girar(int x,int y);
    void girarExaminar(int x,int y);
    void setEye(const Tupla3f & eye);
    void setAt(const Tupla3f & at);
    void setRight(const float & r);
    void setLeft(const float & l);
    void setTop(const float & t);
    void setBottom(const float & b);
    void setSeleccion(const bool & o);
    float getTop();
    float getBottom();
    bool getSeleccion();
    Tupla3f getEyeDefecto();
    Tupla3f getAtDefecto();

};


#endif
