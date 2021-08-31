//
// Created by raul on 15/12/20.
//

#include "Textura.h"

Textura::Textura(std::string archivo){
    jpg::Imagen * pimg = NULL;
    pimg = new jpg::Imagen(archivo);
    width = pimg->tamX();
    height = pimg->tamY();

    //data = pimg->leerPixels();
    for(int i = 0; i < height ; i++) {
        for (int j = 0; j < width; j++) {
            data.push_back(*(pimg->leerPixel(j,height - 1 - i)));
            data.push_back(*(pimg->leerPixel(j,height - 1 - i)+1));
            data.push_back(*(pimg->leerPixel(j,height - 1 - i)+2));
        }
    }
}

void Textura::activar() {
    glEnable(GL_TEXTURE_2D);

    //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if( textura_id == 0 ) {
        glGenTextures(1, &textura_id);
        glBindTexture(GL_TEXTURE_2D, textura_id);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    }
    glBindTexture(GL_TEXTURE_2D, textura_id);
}