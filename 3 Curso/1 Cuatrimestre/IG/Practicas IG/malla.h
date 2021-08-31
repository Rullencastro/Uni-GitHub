// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: Malla3D.h
// -- declaraciones de clase Malla3D (mallas indexadas) y derivados
//
// #############################################################################

#ifndef MALLA3D_H_INCLUDED
#define MALLA3D_H_INCLUDED

#include "aux.h"
#include <cmath>
#include "Material.h"
#include "Textura.h"

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************

class Malla3D
{
   public:

   GLuint id_vbo_ver = 0 , id_vbo_tri = 0 ,id_vbo_tripar = 0 ,id_vbo_triimpar = 0;
   GLuint id_vbo_colp = 0,id_vbo_coll = 0,id_vbo_cols = 0,id_vbo_colpar = 0,id_vbo_colimpar = 0;
   GLuint id_vbo_tri_tapas = 0,id_vbo_tripar_tapas = 0,id_vbo_triimpar_tapas = 0;
   GLuint id_vbo_nor = 0;
   GLuint id_vbo_tex = 0;
   GLuint id_vbo_colSel = 0;

   unsigned int modo_activo = 0;

   void draw_ModoInmediato();

   GLuint CrearVBO (GLuint tipo_vbo, GLuint tamanio_bytes , GLvoid * puntero_ram );

   void draw_ModoDiferido();

   void draw(unsigned int mode,unsigned int color);

   void draw_Suavizado();

   void draw_Plana();

   void inicializarColores();

   void setMaterial(Material mat);

   void setTextura(std::string archivo);

   void cambiarColor(Tupla3f c);

   void cambiarColorSel(Tupla3f c);

   void colorDefecto();

   void materialDefecto();

   void setMaterialEsp(Material mat);

   void setColorEsp(Tupla3f c);


   protected:

   Material m , m_aux;
   Tupla3f c_aux;

   void calcular_normales() ; // calcula tabla de normales de vértices (práctica 3)

   std::vector<Tupla3f> v ;   // tabla de coordenadas de vértices (una tupla por vértice, con tres floats)
   std::vector<Tupla3i> f,fp,fi,ft,fpt,fit ; // una terna de 3 enteros por cada cara o triángulo

   // completar: tabla de colores, tabla de normales de vértices
   std::vector<Tupla3f> cvp,cvl,cvs,cvpar,cvimpar,cvSel;
   std::vector<Tupla3f> nv;

   Textura * textura = nullptr;
   std::string textur = "NINGUNA";


   std::vector<Tupla2f> ct;

} ;


#endif
