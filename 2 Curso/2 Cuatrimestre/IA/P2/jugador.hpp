#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;
  bool zapatillas;
  bool bikini;
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      copiaDestino = destino;
      actual.bikini = false;
      actual.zapatillas = false;
      hayplan=false;
      contPaso=0;
      contRecarga=0;
      recargaEnc=false;
      filRec= -1;
      colRec= -1;
      irRec = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      copiaDestino = destino;
      actual.bikini = false;
      actual.zapatillas = false;
      hayplan=false;
      contPaso=0;
      contRecarga=0;
      recargaEnc=false;
      filRec= -1;
      colRec= -1;
      irRec = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula,filRec,colRec;
    unsigned int contPaso,contRecarga;
    estado actual, destino,copiaDestino;
    list<Action> plan;
    bool hayplan,recargaEnc,irRec;
    Action ultimaAccion;

    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_CosteUniforme(const estado &origen, const estado &destino, list<Action> &plan);
    int costeMovimiento(const estado &a);
    unsigned int siguientePaso(Sensores s);

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);

};

#endif
