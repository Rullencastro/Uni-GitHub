#include "simulador.h"

using namespace std;
using namespace miniwin;

void Simulador::PintarMoviles(const ConjuntoParticulas & a){
    int N = a.GetNroParticulas();
    Particula p;
    for (int i = 0; i < N; i++) {
        p = a.ObtieneParticula(i);
        color(p.getColor());
        circulo_lleno(p.getX(), p.getY(), RADIO);    
}
}
    
void Simulador::PintarFijas(const ConjuntoParticulas & a){
    int N = a.GetNroParticulas();
    Particula p;
    for (int i = 0; i < N; i++) {
        p = a.ObtieneParticula(i);
        color(p.getColor());
        rectangulo_lleno(p.getX()-RADIO, p.getY()-RADIO,p.getX()+RADIO,p.getY()+RADIO );
}
}

Simulador::Simulador(ConjuntoParticulas a , ConjuntoParticulas b, int alt,int anch):moviles(a),fijas(b)
{
       ancho=anch;
       alto=alt;
       vredimensiona(ancho,alto);
       moviles = a;
       fijas = b;
       for(int i = 0 ; i < fijas.GetNroParticulas();i++){
           fijas.ObtieneParticula(i).setDX(0);
           fijas.ObtieneParticula(i).setDY(0);
       }       
}

Simulador::~Simulador(){
    vcierra();
}

void Simulador::SetParticulasMoviles(ConjuntoParticulas a){
    this->moviles = a;
}
void Simulador::SetParticulasFijas(ConjuntoParticulas a){
    this->fijas = a;
}
ConjuntoParticulas Simulador::GetParticulasMoviles()const{
    return(moviles);
}
ConjuntoParticulas Simulador::GetParticulasFijas()const{
    return(fijas);
}
void Simulador::Pintar(int a){
    borra();
    PintarMoviles(moviles);
    PintarFijas(fijas);
    refresca();
    espera(a);
}
void Simulador::Step(){
    moviles.Mover(ancho,alto);
    moviles.GestionarChoques();
    Rebotes();
    std::cout<< moviles;
}
void Simulador::Rebotes(){
    Particula P;
    for(int i=0;i < moviles.GetNroParticulas();i++){
        for(int j=0;j < fijas.GetNroParticulas();j++){
            if(moviles.ObtieneParticula(i).colision(fijas.ObtieneParticula(j))){
                P=moviles.ObtieneParticula(i);
                P.setDX((P.getDX()*(-1))); 
                P.setDY((P.getDY()*(-1)));
                moviles.ReemplazaParticula(i,P);
                
            }
        }    
    }
}

void Simulador::anadirMoviles (int t){
    if(t == ARRIBA){
        Particula A;
        moviles.AgregaParticula(A);    
    }
}

void Simulador::borrarMoviles (int t){
    if(t == ABAJO){
        moviles.BorraParticula((rand()%moviles.GetNroParticulas()));    
    }
}
void Simulador::anadirFijas (int t){
    if(t == DERECHA){
        Particula A;
        fijas.AgregaParticula(A);    
    }
}

void Simulador::borrarFijas (int t){
    if(t == IZQUIERDA){
        fijas.BorraParticula((rand()%fijas.GetNroParticulas()));    
    }
}
