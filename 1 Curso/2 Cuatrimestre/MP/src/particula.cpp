#include "particula.h"

using namespace std;
using namespace miniwin;


    
    void Particula::setX(float a){
        x = a;
    }
    
    void Particula::setY(float a){
        y = a;
    }
    
    void Particula::setDX(float a){
        dx = a;
    }
    
    void Particula::setDY(float a){
        dy = a;
    }
    
    float Particula::getX()const{
        return (x);
    }
    
    float Particula::getY()const{
        return (y);
    }
    
    float Particula::getDX()const{
        return (dx);
    }
    
    float Particula::getDY()const{
        return (dy);
    }
    
    void Particula::setColor(Color a){
        c = a;
    }
    
    Color Particula::getColor()const{
        return(c);
    }
    
    Particula::Particula(){
        Color v[7]={ROJO, VERDE, AZUL,AMARILLO, MAGENTA, CYAN, BLANCO};
        setX((rand()%MIN_X));
        setY((rand()%MIN_Y));
        setDY((rand()%MAX_VEL+1));
        setDX((rand()%MAX_VEL+1));
        setColor(v[(rand()%7)]);
        
    }
    
    Particula::Particula(float a, float b){
        setX(a);
        setY(b);
        setDX(1.0);
        setDY(1.0);
        setColor(ROJO);   
    }
    
    
    float Particula::distancia(Particula a){
        float distancia;
        distancia = sqrt((getX() - a.getX())*(getX() - a.getX()) + (getY() - a.getY())*(getY() - a.getY()));
        return(distancia);
    }
    
    bool Particula::colision (Particula a){
        bool coli = false;
        if(distancia(a) <= UMBRAL){
            coli = true;  
        }
        return(coli);
    }
    
    
    
    void Particula::Mover(int alto,int ancho) {
   const float factor = 1;
   setX(getX() + getDX());
   setY(getY() + getDY());
   if (getX() > ancho - RADIO) {
      setDX(-(getDX())*factor);
      setX(ancho - RADIO);
   }
   else if (getX() < RADIO) {
      setDX(-(getDX())*factor); 
      setX(RADIO); 
   }
   else if (getY() > alto - RADIO) {
      setDY(-(getDY())*factor);
      setY(alto-RADIO);
   }
   else if (getY() < RADIO) {
      setDY(-(getDY())*factor);
      setY(RADIO);
   }
   /*Para la gravedad
   setDY(getDY()+0.2);
    */
   }
    
    void Particula::Rebotar(Particula & otra){
        float aux,auy;
        if(colision(otra)){
            aux=getDX();
            auy=getDY();
            setDX(otra.getDX());
            setDY(otra.getDY());
            otra.setDX(aux);
            otra.setDY(auy);
        }      
    }
    


