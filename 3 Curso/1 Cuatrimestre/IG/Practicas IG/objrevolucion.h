// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: Cubo.h

//
// #############################################################################

#ifndef OBJREVOLUCION_H_INCLUDED
#define OBJREVOLUCION_H_INCLUDED

#include "aux.h"
#include "malla.h"
#include "ply_reader.h"
#include <bits/stdc++.h>
#include <algorithm>

// *****************************************************************************
//
// clases derivadas de Malla3D (definen constructores específicos)
//
// *****************************************************************************

// *****************************************************************************
// Cubo con centro en el origen y lado unidad por defecto
// (tiene 9 vertices y 6 caras)

class ObjRevolucion : public Malla3D
{
   public:
       ObjRevolucion();
       ObjRevolucion(const std::string & archivo, int num_instancias, bool tapa_sup=true, bool tapa_inf=true,char eje = 'Y',bool textura = false) ;
       ObjRevolucion(std::vector<Tupla3f> archivo, int num_instancias, bool tapa_sup=true, bool tapa_inf=true, char eje = 'Y',bool textura = false) ;
       void draw_ModoDiferido_Revolucion(bool conTapas);
       void draw_ModoInmediato_Revolucion(bool conTapas);
       void draw_Suavizado_Revolucion(bool conTapas);
       void draw_Plana_Revolucion(bool conTapas);
       void draw(unsigned int mode,unsigned int color,bool conTapas);
       void asignarTexCoord(float N,float M);

protected:
    void crearMalla(std::vector<Tupla3f> perfil_original, int num_instancias, bool conTapas,char eje, bool textura);
    Tupla3f tapaSuperior = {1,0,0} ,tapaInferior = {1,0,0};
    bool ts = true, ti = true;
    unsigned int modo_activo;
} ;




#endif
