
#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "miniwin.h"
#include "definiciones.h"
#include "particula.h"
#include "conjuntoparticulas.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
using namespace miniwin;

class Simulador{
private:
    ConjuntoParticulas moviles;
    ConjuntoParticulas fijas;
    int ancho , alto;
    void PintarMoviles(const ConjuntoParticulas & a);
    void PintarFijas(const ConjuntoParticulas & a);
public:
    Simulador(ConjuntoParticulas a, ConjuntoParticulas b, int alt,int anch);
    ~Simulador();
    void SetParticulasMoviles(ConjuntoParticulas a);
    void SetParticulasFijas(ConjuntoParticulas a);
    ConjuntoParticulas GetParticulasMoviles()const;
    ConjuntoParticulas GetParticulasFijas()const;
    void Pintar(int a);
    void Step();
    void Rebotes();
    void anadirMoviles (int t);
    void borrarMoviles (int t);
    void anadirFijas (int t);
    void borrarFijas (int t);
};



#endif /* SIMULADOR_H */

