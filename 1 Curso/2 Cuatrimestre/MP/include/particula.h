#ifndef PARTICULA_H
#define PARTICULA_H

#include "miniwin.h"
#include "definiciones.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
using namespace miniwin;

class Particula {
private:
   float x, y;
   float dx, dy;
   Color c;
public:

    void setX(float a);
    void setY(float a);
    void setDX(float a);
    void setDY(float a);
    float getX()const;
    float getY()const;
    float getDX()const;
    float getDY()const;
    void setColor(Color a);
    Color getColor()const;
    Particula();
    Particula(float a, float b);
    float distancia(Particula a);
    bool colision (Particula a);
    void Mover(int alto,int ancho);
    void Rebotar(Particula & otra);
};


#endif /* PARTICULA_H */

