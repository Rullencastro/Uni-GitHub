#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "malla.h"
#include "cubo.h"
#include "tetraedro.h"
#include "objply.h"
#include "objrevolucion.h"
#include "cilindro.h"
#include "cono.h"
#include "esfera.h"
#include "semiesfera.h"
#include "Material.h"
#include "LuzPosicional.h"
#include "LuzDireccional.h"
#include "Gallina.h"
#include "Cuadro.h"
#include "Camara.h"

typedef enum {NADA,SELVISUALIZACION,SELDIBUJADO,ILUMINACION,ANIMANUAL,ANIMAUTO,CAMARAS} menu;
typedef enum {MOVIENDO_CAMARA_FIRSTPERSON,MOVIENDO_CAMARA_EXAMINAR,NO_MOVIMIENTO} estadosMouse;
class Escena
{

   private:

    bool puntos = false, lineas = false, solido = false,ajedrez = false;
    bool tapas =true;
    bool ilum = false,luz1 = false, luz2 = false, v_alpha = false, v_beta = false;
    bool anim_manual = false,signo = true,anim_auto = false ;
    bool anim_luz = false;
    unsigned int dibuj = 1, color = 2 , gradoLibertad = 0, grado = 0;
    float mov1 = 0.0,mov2 = 0.0,mov3 = 0.0,mov4 = 0.0,mov5 = 0.0,mov6 = 0.0,mov7 = 0.0,mov8 = 0.0;
    int contadorTembleque = 0,contadorTembleque2 = 0;
    int camaraActiva = 0;
    float aux = 1.0;

    std::vector<Tupla3f> cSel;

 // ** PARÁMETROS DE LA CÁMARA (PROVISIONAL)
       
       // variables que definen la posicion de la camara en coordenadas polares
   GLfloat Observer_distance;
   GLfloat Observer_angle_x;
   GLfloat Observer_angle_y;

   // variables que controlan la ventana y la transformacion de perspectiva
   GLfloat Width, Height, Front_plane, Back_plane;

    // Transformación de cámara
	void change_projection();
	void change_observer();



   void clear_window();

   menu modoMenu=NADA;
   // Objetos de la escena
   Ejes ejes;
   Cubo * cubo = nullptr ; // es importante inicializarlo a 'nullptr'
   Tetraedro * tetraedro= nullptr ; // es importante inicializarlo a 'nullptr'

   unsigned int N = 0;

   ObjRevolucion * lata = nullptr;
    ObjRevolucion * tapa_sup_lata = nullptr;
    ObjRevolucion * tapa_inf_lata = nullptr;
    Esfera * sol = nullptr;
    Semiesfera * huevo = nullptr;


    LuzPosicional * lp = nullptr;
   LuzDireccional * ld = nullptr;
   Material obsidiana,mdifuso,mespecular;

   Gallina * gallina = nullptr;

   Cuadro * cuadro = nullptr;

   std::vector<Camara> camaras;

   public:

    estadosMouse estadoRaton = MOVIENDO_CAMARA_FIRSTPERSON;
    int xant, yant;

    Escena();
	void inicializar( int UI_window_width, int UI_window_height );
	void redimensionar( int newWidth, int newHeight ) ;

	// Dibujar
	void dibujar() ;
    void dibujar_encapsulado(unsigned int visualizacion);

	// Interacción con la escena
	bool teclaPulsada( unsigned char Tecla1, int x, int y ) ;
	void teclaEspecial( int Tecla1, int x, int y );

	void animarModeloJerarquico();

	void animarLuzPuntual();

	void clickRaton(int boton , int estado , int x, int y );
    void ratonMovido(int x , int y);
    void zoomCamaraActiva(char c);
    void dibujaSeleccion();
    void procesamientoPick(int x, int y);



    };
#endif
