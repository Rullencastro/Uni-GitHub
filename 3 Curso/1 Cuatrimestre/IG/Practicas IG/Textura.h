//
// Created by raul on 15/12/20.
//

#ifndef PRACTICAS_IG_TEXTURA_H
#define PRACTICAS_IG_TEXTURA_H

#include "aux.h"
#include "jpg_imagen.hpp"



class Textura {
private:
    GLuint textura_id = 0;
    std::vector<unsigned char> data;
    int width , height;
public:
    Textura(std::string archivo);
    void activar();
};


#endif //PRACTICAS_IG_TEXTURA_H
