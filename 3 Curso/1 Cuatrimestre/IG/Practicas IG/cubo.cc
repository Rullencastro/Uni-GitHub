#include "aux.h"
#include "cubo.h"

Cubo::Cubo(float lado)
{

   // inicializar la tabla de vértices
   Tupla3f v1({lado/2,0,-lado/2}), v2({lado/2,lado,-lado/2}), v3({lado/2,0,lado/2}), v4({lado/2,lado,lado/2}),v5({-lado/2,lado,-lado/2}), v6({-lado/2,lado,lado/2}), v7({-lado/2,0,lado/2}), v8({-lado/2,0,-lado/2});
   v={v1,v2,v3,v4,v5,v6,v7,v8};
   // inicializar la tabla de caras o triángulos:
   Tupla3i t0({0,1,2}), t1({2,1,3}), t2({3,1,4}), t3({4,5,3}) ,t4({3,5,2}),t5({6,2,5}),t6({1,0,4}),t7({4,0,7}),t8({0,6,7}),t9({0,2,6}),t10({7,5,4}),t11({7,6,5});
   f={t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11};
   fp={t0,t2,t4,t6,t8,t10};
   fi={t1,t3,t5,t7,t9,t11};

   calcular_normales();

   inicializarColores();
}

