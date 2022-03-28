#include "conjuntoparticulas.h"

using namespace std;
using namespace miniwin;

void ConjuntoParticulas::reservarMemoria(int a){
    capacidad=0;
    if(a>0){
    capacidad=a;
    set = new Particula [a];
    }
}

void ConjuntoParticulas::liberarMemoria(){
    if(set != 0){
    delete [] set;
    set=0;
    capacidad=0;
    utiles=0;
    }
}

void ConjuntoParticulas::redimensiona(int a){
    Particula *aux=0;
    aux=new Particula [a];
    int l = utiles;
    if(a<=l){
   
        l = a;
        setUtil(a);
           
    }
    
      for(int i=0 ;i < this->utiles ;i++){
        aux[i]= set[i];
    }  
    setCap(a);
    if(set != 0){
    delete [] set;
    }
    set=aux;
       
}

ConjuntoParticulas::ConjuntoParticulas(){
    set = 0;
    capacidad=0;
    utiles=0;
}

ConjuntoParticulas::ConjuntoParticulas(int c){
    reservarMemoria(c);
    capacidad=c;
    utiles=c;    
}
ConjuntoParticulas::ConjuntoParticulas(const ConjuntoParticulas & a){
    capacidad = a.getCap();
    utiles = a.GetNroParticulas();
    reservarMemoria(capacidad);
    for(int i=0;i < utiles;i++){
            set[i]=a.ObtieneParticula(i);
    }    
}

ConjuntoParticulas::~ConjuntoParticulas(){
    liberarMemoria();
}

int ConjuntoParticulas::getCap()const{
    return(capacidad);
}
int ConjuntoParticulas::GetNroParticulas()const{
    return(utiles);
}
void ConjuntoParticulas::setCap(int a){
    capacidad = a;
}
void ConjuntoParticulas::setUtil(int a){
    utiles = a;
}

void ConjuntoParticulas::AgregaParticula(Particula A){
    int c;
    if(utiles < capacidad){
        utiles++;
        set[utiles-1]=A;
    }
    else if(utiles == capacidad){
        c = capacidad + TAM_BLOQUE;
        redimensiona(c);
        utiles++;
        set[utiles-1]=A;
    }
}


void ConjuntoParticulas::BorraParticula(int a){
    for(int i=a;i < utiles - 1;i++){
        set[i]=set[i + 1];
    }
    utiles--;
    if ((capacidad - utiles) > TAM_BLOQUE){
        redimensiona(this->capacidad - TAM_BLOQUE);       
    }
}

Particula ConjuntoParticulas::ObtieneParticula(int a)const{
    return(set[a]);
}

void ConjuntoParticulas::ReemplazaParticula (int a, Particula P){
    set[a]=P;   
}

void ConjuntoParticulas::Mover(int ancho,int alto){
    for(int i=0; i < utiles ;i++){
        set[i].Mover(alto,ancho);
    }
}

void ConjuntoParticulas::GestionarChoques(){
    for(int i=0; i < utiles ;i++){
        for(int j=i+1; j < utiles-1;j++){
            if(set[i].colision(set[j])){
             set[i].Rebotar(set[j]);   
            }
        }
    }
}

ConjuntoParticulas  ConjuntoParticulas::operator+(const ConjuntoParticulas & c)const{
    ConjuntoParticulas aux;
    for(int i=0;i<utiles;i++){
        aux.AgregaParticula(set[i]);
    }
    for(int i=0;i<c.GetNroParticulas();i++){
        aux.AgregaParticula(c.ObtieneParticula(i));
    }
    return (aux);
}

ConjuntoParticulas & ConjuntoParticulas::operator=(const ConjuntoParticulas & c){
    
    if(&c != this){
        liberarMemoria();
        capacidad = c.getCap();
        utiles = c.GetNroParticulas();
        reservarMemoria(capacidad);
        for(int i=0;i < utiles;i++){
            set[i]=c.ObtieneParticula(i);
        }
    }
    return(*this);
}

ostream & operator<<(ostream & os,const ConjuntoParticulas & c){
    os << "Capacidad: "<< c.getCap()<< endl;
    os << "Nro de Particulas: " << c.GetNroParticulas() << endl;
    for(int i=0 ; i < c.GetNroParticulas();i++){
        os << "Particula: "<< i << "  \t(\t "<< c.ObtieneParticula(i).getX() << ",\t"<< c.ObtieneParticula(i).getY() << ")"<<"\t V(x): "<<c.ObtieneParticula(i).getDX()<<"\t V(y): "<<c.ObtieneParticula(i).getDY()<<endl ;
        
    }
    return(os);
}

