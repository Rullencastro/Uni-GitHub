//
// Created by raul on 5/11/20.
//

#include "cono.h"
#include "cmath"

Cono::Cono(int num_ver_perfil, int num_instancias_per,float h , float r ,bool textura){
    tapaInferior = {0,0,0};
    tapaSuperior = {0, h,0};
    std::vector<Tupla3f> perfil;
    float altura_vertice = h/num_ver_perfil;
    for(int i = num_ver_perfil; i > 0; --i) {
        perfil.push_back({r*i/num_ver_perfil, h-altura_vertice*i, 0});
    }

    crearMalla(perfil,num_instancias_per,true,'y',textura);
    calcular_normales();

    for(int i = 0; i < f.size(); i++){
        if(i > num_instancias_per)
            ft.push_back(f[i]);
        if(i % 2 == 0) {
            fp.push_back(f[i]);
            if(i > num_instancias_per)
                fpt.push_back(f[i]);
        }
        else {
            fi.push_back(f[i]);
            if(i > num_instancias_per)
                fit.push_back(f[i]);
        }
    }

    inicializarColores();
}