#include "objply.h"
#include "ply_reader.h"
 

// *****************************************************************************
//
// Clase ObjPLY (práctica 2)
//
// *****************************************************************************

ObjPLY::ObjPLY( const std::string & nombre_archivo )
{
   // leer la lista de caras y vértices
   ply::read( nombre_archivo, this->v, this->f );

   for(int i = 0; i < f.size(); i++){
       if(i % 2 == 0)
            fp.push_back(f[i]);
       else
            fi.push_back(f[i]);
   }
   calcular_normales();

    Tupla3f c1({1,0,0}), c2({0,1,0}) , c3({0.5,0.5,0.5}),c4({0,0,0}),c5({0.5,0,0.5});
    for (int i = 0; i < v.size(); i++) {
        cvpar.push_back(c1);
        cvimpar.push_back(c2);
        cvp.push_back(c5);
        cvl.push_back(c4);
        cvs.push_back(c3);
    }
}


