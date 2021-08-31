//
// Created by raul on 30/11/20.
//

#include "semiesfera.h"
Semiesfera::Semiesfera(int num_ver_perfil, int num_instancias_per, float r,bool textura) {
    tapaInferior = {0,-r,0};
    tapaSuperior = {0, -r,0};
    std::vector<Tupla3f> perfil;
    const float PI = 3.141592653589793238462;
    float angulo = PI/num_ver_perfil;
    for(int i = 1;i < num_ver_perfil/2;i++){
        perfil.push_back({ 0 * (float)cos(angulo*i) - (-r) * (float)sin(angulo*i),0 * (float)sin(angulo*i) + (-r) * (float)cos(angulo*i) ,0});
    }
    for(int i = num_ver_perfil/2;i >= 1;i--){
        perfil.push_back({ 0 * (float)cos(angulo*i) - (-r) * (float)sin(angulo*i),0 * (float)sin(angulo*i) + (-r) * (float)cos(angulo*i) ,0});
    }

    crearMalla(perfil,num_instancias_per,true,'y',textura);
    calcular_normales();

    for(int i = 0; i < f.size(); i++){
        if(i > num_instancias_per*2)
            ft.push_back(f[i]);
        if(i % 2 == 0) {
            fp.push_back(f[i]);
            if(i > num_instancias_per*2)
                fpt.push_back(f[i]);
        }
        else {
            fi.push_back(f[i]);
            if(i > num_instancias_per*2)
                fit.push_back(f[i]);
        }
    }

    inicializarColores();

}