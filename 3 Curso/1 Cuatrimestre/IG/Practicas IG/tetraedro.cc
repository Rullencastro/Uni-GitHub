#include "aux.h"
#include "malla.h"
#include "tetraedro.h"
#include <cmath>

Tetraedro::Tetraedro(float lado)
{
    float h = sqrtf( pow(lado,2) - pow((lado/2),2) );
    float H = sqrtf( pow(lado,2) - pow(h * (float)(2.0/3.0)  , 2 ) );
   // inicializar la tabla de vértices
   Tupla3f v1({0,0,- h * (float)(2.0/3.0) }), v2({0,H,0}), v3({lado/2,0,h * (float)(1.0/3.0)}), v4({-lado/2,0,h * (float)(1.0/3.0)});
   v={v1,v2,v3,v4};
    
   // inicializar la tabla de caras o triángulos:
   Tupla3i t0({3,1,0}), t1({3,2,1}), t2({3,0,2}), t3({2,0,1});
   f={t0,t1,t2,t3};
   fp={t0,t2};
   fi={t1,t3};

    calcular_normales();

   // (es importante en cada cara ordenar los vértices en sentido contrario
   //  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

   inicializarColores();

   Tupla2f ct1({0,0}) , ct2({1,0}) , ct3({0,1}), ct4({1,1});
   ct = {ct1,ct2,ct3,ct4};


}



