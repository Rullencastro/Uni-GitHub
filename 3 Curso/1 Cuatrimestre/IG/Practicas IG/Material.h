//
// Created by raul on 22/11/20.
//

#ifndef PRACTICAS_IG_MATERIAL_H
#define PRACTICAS_IG_MATERIAL_H

#include "aux.h"


class Material {
private :
    Tupla4f difuso ;
    Tupla4f especular ;
    Tupla4f ambiente ;
    float brillo ;
public :
    Material();
    Material ( Tupla4f mdifuso ,Tupla4f mespecular ,Tupla4f mambiente ,float brillo );
    void aplicar ();
    void setMaterial(Material mat);
};


#endif //PRACTICAS_IG_MATERIAL_H
