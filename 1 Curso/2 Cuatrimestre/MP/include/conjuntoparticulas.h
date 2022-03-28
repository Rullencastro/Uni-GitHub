#ifndef CONJUNTOPARTICULAS_H
#define CONJUNTOPARTICULAS_H

#include "miniwin.h"
#include "definiciones.h"
#include "particula.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
using namespace miniwin;

const int TAM_BLOQUE = 5;

class ConjuntoParticulas {
private:
    Particula *set;
    int capacidad;
    int utiles;
    void reservarMemoria(int a);
    void liberarMemoria();
    void redimensiona(int a);
public:
    ConjuntoParticulas();
    ConjuntoParticulas(int c);
    ConjuntoParticulas(const ConjuntoParticulas  & a);
    ~ConjuntoParticulas();
    int getCap()const;
    int GetNroParticulas()const;
    void setCap(int a);
    void setUtil(int a);
    void AgregaParticula(Particula A);
    void BorraParticula(int a);
    Particula ObtieneParticula (int a)const;
    void ReemplazaParticula (int a,Particula P);
    void Mover(int alto,int ancho);
    void GestionarChoques();
    ConjuntoParticulas  operator+(const ConjuntoParticulas & c)const;
    ConjuntoParticulas & operator=(const ConjuntoParticulas & c);
    friend ostream & operator<<(ostream & os,const ConjuntoParticulas & c);
    
};
ostream & operator<<(ostream & os,const ConjuntoParticulas & c);







#endif /* CONJUNTOPARTICULAS_H */

