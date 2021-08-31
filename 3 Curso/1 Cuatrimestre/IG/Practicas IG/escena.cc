

#include "aux.h"     // includes de OpenGL/glut/glew, windows, y librería std de C++
#include "escena.h"
#include "malla.h" // objetos: Cubo y otros....

//**************************************************************************
// constructor de la escena (no puede usar ordenes de OpenGL)
//**************************************************************************

Escena::Escena()
{
    Front_plane       = 50.0;
    Back_plane        = 2000.0;
    Observer_distance = 4*Front_plane;
    Observer_angle_x  = 0.0 ;
    Observer_angle_y  = 0.0 ;

    ejes.changeAxisSize( 5000 );

    // crear los objetos de la escena....
    // .......completar: ...
   
      tetraedro = new Tetraedro(50);
      cubo = new Cubo(40);
      N = 50;

      lata = new ObjRevolucion("plys/lata-pcue.ply",N,false,false,'y',true);
      tapa_sup_lata = new ObjRevolucion("plys/lata-psup.ply",N,false,false,'y',true);
      tapa_inf_lata = new ObjRevolucion("plys/lata-pinf.ply",N,false,false,'y',true);


      lp = new LuzPosicional({-100.0,120.0,90.0},GL_LIGHT1,{1.0,1.0,1.0,1.0},{1.0,1.0,1.0,1.0},{1.0,1.0,1.0,1.0});
      ld = new LuzDireccional({90.0,180.0},GL_LIGHT2,{1.0,1.0,1.0,1.0},{1.0,1.0,1.0,1.0},{1.0,1.0,1.0,1.0});
      obsidiana = Material({0.18275,0.17,0.22525,1.0},{0.332741,0.328634,0.346435,1.0},{0.05375,0.05,0.06625,1.0},0.3);
      mdifuso = Material({0.9,0.9,0.9,0.9},{0.0,0.0,0.0,0.0},{0.8,0.8,0.8,0.8},0.0);
      mespecular = Material({0.0,0.0,0.0,1.0}, {1.0,1.0,1.0,1.0}, {0.0,0.0,0.0,0.0}, 1.0);
      Material plata = Material({0.50754,0.50754,0.50754,1.0},{0.508273,0.508273,0.508273,1.0},{0.19225,0.19225,0.19225,1.0},0.4);
      Material ruby = Material({0.61424,0.04136,0.04136,1.0},{0.727811,0.626959,0.626959,1.0},{0.1745,0.01175,0.01175,1.0},0.6);
      Material oro = Material({0.75164,0.60648,0.22648,1.0},{0.628281,0.555802,0.366065,1.0},{0.24725,0.1995,0.0745,1.0},0.4);


      gallina = new Gallina();

      //HUEVO
      huevo = new Semiesfera(10,50,1, false);
      huevo->setMaterial(oro);
      huevo->cambiarColor({1,1,0});
      huevo->cambiarColorSel({1,1,0});

      //CUADRO
      cuadro = new Cuadro(400);
      cuadro->setTextura("texturas/campo2.jpg");

      //LATA
      lata->cambiarColor({1,1,1});
      lata->cambiarColorSel({1,0,0});
      lata->setTextura("texturas/text-lata-1.jpg");
      lata->setMaterial(mespecular);
      tapa_sup_lata->cambiarColor({1,1,1});
      tapa_sup_lata->cambiarColorSel({1,0,0});
      tapa_sup_lata->setTextura("texturas/lata-tapas.jpg");
      tapa_sup_lata->setMaterial(mespecular);
      tapa_inf_lata->cambiarColor({1,1,1});
      tapa_inf_lata->cambiarColorSel({1,0,0});
      tapa_inf_lata->setTextura("texturas/lata-tapas.jpg");
      tapa_inf_lata->setMaterial(mespecular);


      sol = new Esfera (10,20,1,false);
      sol->cambiarColor({1,1,1});
      sol->cambiarColorSel({0,0,1});
      sol->setTextura("texturas/luna.jpg");
      sol->setMaterial(mespecular);

      Camara * orto = new Camara({0.0,0.0,125.0},{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, 1, -200.0, 200.0, 0.5, 450.0, 200.0,-200.0);
      Camara * perspectiva = new Camara({50.0,0.0,100.0},{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, 0, -1.0, 1.0, 0.5, 500.0, 1.0,-1.0);
      Camara * cam2 = new Camara({-100.0,-100.0,200.0},{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, 1, -100.0, 100.0, 0.5, 500.0, 100.0,-100.0);


      camaras.push_back(*orto);
      camaras.push_back(*perspectiva);
      camaras.push_back(*cam2);

      cSel.push_back({1,1,0});
      cSel.push_back({1,0,0});
      cSel.push_back({0,0,1});


    std::cout << "MENU PRINCIPAL. Elija una de las siguientes opciones:"<<std::endl;
    std::cout << "\tV - Modo Selección de Visualización\n\tD - Modo Selección de Dibujado\n\tM - Modo Animacion Manual\n\tA - Modo Animacion Automático\n\tC - Modo Seleccion de Camaras\n\tQ - Salir" << std::endl;
   
}

//**************************************************************************
// inicialización de la escena (se ejecuta cuando ya se ha creado la ventana, por
// tanto sí puede ejecutar ordenes de OpenGL)
// Principalmemnte, inicializa OpenGL y la transf. de vista y proyección
//**************************************************************************

void Escena::inicializar( int UI_window_width, int UI_window_height )
{
	glClearColor( 1.0, 1.0, 1.0, 1.0 );// se indica cual sera el color para limpiar la ventana	(r,v,a,al)

	glEnable( GL_DEPTH_TEST );	// se habilita el z-bufer
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);


    change_projection();
    glViewport( 0, 0, UI_window_width, UI_window_height );
}



// **************************************************************************
//
// función de dibujo de la escena: limpia ventana, fija cámara, dibuja ejes,
// y dibuja los objetos
//
// **************************************************************************

void Escena::dibujar()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Limpiar la pantalla
	change_observer();
	glDisable(GL_LIGHTING);
    ejes.draw();


   if(ilum){
       glEnable(GL_LIGHTING);
       if(color == 4)
           glShadeModel(GL_SMOOTH);
       else if(color == 5)
           glShadeModel(GL_FLAT);

       if(luz1) {
           lp->activar();
           dibujar_encapsulado(color);
       }else
           lp->desactivar();
       if(luz2){
           ld->activar();
           dibujar_encapsulado(color);
       }else
           ld->desactivar();
   }

   if(lineas || puntos || solido || ajedrez){
       glDisable(GL_LIGHTING);
      if (puntos && !ajedrez){
         glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
         glPointSize(5);
         dibujar_encapsulado(0);
      }

      if(lineas && !ajedrez){
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         dibujar_encapsulado(1);
      }

      if(solido && !ajedrez){
          glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
          dibujar_encapsulado(2);
      }
      
      if(ajedrez) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          dibujar_encapsulado(3);
      }
   }
}

//**************************************************************************
//
// función que se invoca cuando se pulsa una tecla
// Devuelve true si se ha pulsado la tecla para terminar el programa (Q),
// devuelve false en otro caso.
//
//**************************************************************************

void Escena::dibujar_encapsulado(unsigned int visualizacion)
{
    using namespace std;
    if(anim_manual) {
        gallina->animacion_manual(gradoLibertad,signo);
    }
    anim_manual = false;

    glPushMatrix();
    glTranslatef(0, -200, 0);
    cuadro->draw(dibuj,visualizacion);
    glPopMatrix();


    if(mov1 < 150) {
        mov1 += 0.3;
        anim_auto = true;
    }

    if(mov1 >= 150 && contadorTembleque < 40) {
        if (mov2 >= 5 || mov2 < 0){
            aux = -aux;
            contadorTembleque++;
        }
            mov2 += aux ;
        anim_auto = false;
    }

    if(mov3 < 40 && contadorTembleque >= 10){
        mov3 += 0.2;
    }

    if(mov3 >= 40 && contadorTembleque2 < 80){
        if (mov4 >= 5 || mov4 < 0){
            aux = -aux;
            contadorTembleque2++;
        }
        mov4 += aux ;
    }

    if(mov3 >= 40 && mov5 < 180){
        mov5 += 1.0;
    }

    if(mov6 < 165 && contadorTembleque2 >= 80){
        mov6 += 1.0;
        mov7 += 0.012;
        anim_auto = true;
    }


    if(contadorTembleque > 1) {
        glPushMatrix();
        glTranslatef(-100+mov3*2, -mov3, 50+mov4);
        glRotatef(90-mov3*2,0,0,1);
        glScalef(15, 15, 15);
        glPushMatrix();
        glTranslatef(-mov7,0,0);
        glRotatef(180+mov6, 0, 0, 1);
        glScalef(1, 1.5, 1);
        huevo->draw(dibuj, visualizacion, tapas);
        glPopMatrix();
        glPushMatrix();
        huevo->draw(dibuj, visualizacion, tapas);
        glTranslatef(0, 0.3, 0);
        glRotatef(-90, 0, 1, 0);
        glScalef(0.33, 0.33, 0.33);
        gallina->draw(dibuj, visualizacion, tapas);
        glPopMatrix();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(-100, -100, 50);
    glRotatef(90,0,0,1);
    glScalef(40, 40, 40);
    lata->draw(dibuj,visualizacion,tapas);
    tapa_sup_lata->draw(dibuj,visualizacion,tapas);
    tapa_inf_lata->draw(dibuj,visualizacion,tapas);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-100, 150, 50);
    glScalef(20, 20, 20);
    sol->draw(dibuj, visualizacion, tapas);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-mov1, 0, mov2);
        glPushMatrix();
        glTranslatef(50, 0, 50);
        glRotatef(-90+mov5,0,1,0);
        glScalef(20, 20, 20);
        gallina->draw(dibuj, visualizacion, tapas);
        glPopMatrix();
    glPopMatrix();

}

void Escena::animarModeloJerarquico() {

    if (anim_auto) {
        gallina->animar();
    }
}

void Escena::animarLuzPuntual(){
    if(anim_luz)
        lp->animar();
}

bool Escena::teclaPulsada( unsigned char tecla, int x, int y )
{
   using namespace std ;
   cout << "Tecla pulsada: '" << tecla << "'" << endl;
   bool salir=false;
   if(modoMenu==NADA) {
       switch (toupper(tecla)) {
           case 'Q' :
               salir = true;
               break;
           case 'V' :
               modoMenu = SELVISUALIZACION;
               cout << "Modo Selección de Visualizacion activado" << endl;
               cout << "\tP - Puntos\n\tL - Lineas\n\tS - Solido\n\tA - Ajedrez\n\tT - Poner/Quitar Tapas\n\tI - Iluminacion\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'D' :
               modoMenu = SELDIBUJADO;
               cout << "Modo Selección de Dibujado activado" << endl;
               cout << "\t1 - Modo Inmediato\n\t2 - Modo Diferido\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'M' :
               modoMenu = ANIMANUAL;
               anim_auto = false;
               cout << "Modo Animacion Manual activado" << endl;
               cout << "\t'+' - Aumentar Valor\n\t'-' - Disminuir Valor\n\t[0..9,A,B] - Grado de Libertad\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'A' :
               modoMenu = ANIMAUTO;
               anim_auto = true;
               cout << "Modo Animacion Automatica activado" << endl;
               cout << "\t'+' - Aumentar Velocidad\n\t'-' - Disminuir Velocidad\n\t[1 - 7] - Seleccionar Grado Libertad\n\tG - Cambiar velocidad GENERAL\n\tQ - Volver al Menu Principal" << endl;               break;
           case 'C' :
               modoMenu = CAMARAS;
               cout << "Modo Selección Camara activado" << endl;
               cout << "\t[0 - 2] - Seleccionar Camara\n\t'+' - Aumentar Zoom\n\t'-' - Disminuir Zoom\n\tQ - Volver al Menu Principal" << endl;
               break;
           default:
               cout << "Tecla sin función en el modo actual" << endl;
               break;
       }
   }
   else if(modoMenu==SELVISUALIZACION) {
       switch (toupper(tecla)) {
           case 'P' :
               if (!puntos) {
                   puntos = true;
                   ilum = false;
                   color = 0;
               } else
                   puntos = false;
               cout << "Puntos seleccionado" << endl;
               break;
           case 'L' :
               if (!lineas) {
                   lineas = true;
                   ilum = false;
                   color = 1;
               } else
                   lineas = false;
               cout << "Lineas seleccionado" << endl;
               break;
           case 'S' :
               if (!solido) {
                   solido = true;
                   ilum = false;
                   color = 2;
               } else
                   solido = false;
               cout << "Solido seleccionado" << endl;
               break;
           case 'A' :
               if (!ajedrez) {
                   ajedrez = true;
                   ilum = false;
                   color = 3;
               } else
                   ajedrez = false;
               cout << "Ajedrez seleccionado" << endl;
               break;
           case 'T':
               if (!tapas) {
                   tapas = true;
               } else
                   tapas = false;
               cout << "Poner/Quitar Tapas seleccionado" << endl;
               break;
           case 'I':
               if(!ilum){
                   ilum = true;
                   ajedrez = false;
                   puntos = false;
                   lineas = false;
                   solido = false;
                   luz1 = true;
                   color = 4;
                   modoMenu = ILUMINACION;
                   cout << "Modo de Visualización con Iluminación activado" << endl;
                   cout << "\t0 - Luz Posicional\n\t1 - Luz Direccional\n\tA - Modificar ángulo alfa\n\tB - Modificar ángulo beta\n\t> - Aumentar angulo seleccionado\n\t< - Decrementar angulo seleccionado\n\tU - Animar Luz Puntual\n\tQ - Volver al Menu Principal" << endl;
               }
               break;
           case 'Q' :
               modoMenu = NADA;
               cout << "MENU PRINCIPAL. Elija una de las siguientes opciones:"<<endl;
               cout << "\tV - Modo Selección de Visualización\n\tD - Modo Selección de Dibujado\n\tM - Modo Animacion Manual\n\tA - Modo Animacion Automático\n\tC - Modo Seleccion de Camaras\n\tQ - Salir" << endl;
               break;
           default:
               cout << "Tecla sin función en el modo actual" << endl;
               break;
       }
   }
   else if(modoMenu==SELDIBUJADO) {
       switch (toupper(tecla)) {
           case '1' :
               dibuj = 1;
               cout << "Modo inmediato seleccionado" << endl;
               cout << "\t1 - Modo Inmediato\n\t2 - Modo Diferido\n\tQ - Volver al Menu Principal" << endl;
               break;
           case '2' :
               dibuj = 2;
               cout << "Modo diferido seleccionado" << endl;
               cout << "\t1 - Modo Inmediato\n\t2 - Modo Diferido\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'Q' :
               modoMenu = NADA;
               cout << "MENU PRINCIPAL. Elija una de las siguientes opciones:"<<endl;
               cout << "\tO - Modo Selección de Objeto\n\tV - Modo Selección de Visualización\n\tD - Modo Selección de Dibujado\n\tM - Modo Animacion Manual\n\tA - Modo Animacion Automático\n\tC - Modo Seleccion de Camaras\n\tQ - Salir" << endl;
               break;
           default:
               cout << "Tecla sin función en el modo actual" << endl;
               break;
       }
   }
   else if(modoMenu==ILUMINACION){
       switch (toupper(tecla)) {
           case '0':
               if (!luz1)
                   luz1 = true;
               else
                   luz1 = false;
               break;
           case '1':
               if (!luz2)
                   luz2 = true;
               else
                   luz2 = false;
               break;
           case 'S':
               color = 4;
               break;
           case 'P':
               color = 5;
               break;
           case 'A':
               if (!v_alpha) {
                   v_alpha = true;
                   v_beta = false;
               } else
                   v_alpha = false;
               break;
           case 'B':
               if (!v_beta) {
                   v_beta = true;
                   v_alpha = false;
               } else
                   v_beta = false;
               break;
           case '>':
               if (!v_alpha && !v_beta)
                   cout << "No se puede incrementar el angulo si no has especificado que angulo. Pulsa A o B" << endl;
               else if (v_alpha)
                   ld->variarAnguloAlpha(5.0);
               else
                   ld->variarAnguloBeta(5.0);
               break;
           case '<':
               if (!v_alpha && !v_beta)
                   cout << "No se puede decrementar el angulo si no has especificado que angulo. Pulsa A o B" << endl;
               else if (v_alpha)
                   ld->variarAnguloAlpha(-5.0);
               else
                   ld->variarAnguloBeta(-5.0);
               break;
           case 'U':
               if (!anim_luz)
                   anim_luz = true;
               else
                   anim_luz = false;
               break;
           case 'Q' :
               modoMenu = NADA;
               cout << "MENU PRINCIPAL. Elija una de las siguientes opciones:" << endl;
               cout<< "\tV - Modo Selección de Visualización\n\tD - Modo Selección de Dibujado\n\tM - Modo Animacion Manual\n\tA - Modo Animacion Automático\n\tC - Modo Seleccion de Camaras\n\tQ - Salir"
                       << endl;
               break;
           default:
               cout << "Tecla sin función en el modo actual" << endl;
               break;
       }
   }
   else if(modoMenu==ANIMANUAL) {
       unsigned int i = tecla;
       switch (toupper(tecla)) {
           case '+' :
               anim_manual = true;
               signo = true;
               cout << "Aumentando valor" << endl;
               cout << "\t'+' - Aumentar Valor\n\t'-' - Disminuir Valor\n\t[0..9,A,B] - Grado de Libertad\n\tQ - Volver al Menu Principal" << endl;
               break;
           case '-' :
               anim_manual = true;
               signo = false;
               cout << "Disminuyendo valor" << endl;
               cout << "\t'+' - Aumentar Valor\n\t'-' - Disminuir Valor\n\t[0..9,A] - Grado de Libertad\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'A' :
               gradoLibertad = 10;
               cout << "\t'+' - Aumentar Valor\n\t'-' - Disminuir Valor\n\t[0..9,A,B] - Grado de Libertad\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'B' :
               gradoLibertad = 11;
               cout << "\t'+' - Aumentar Valor\n\t'-' - Disminuir Valor\n\t[0..9,A,B] - Grado de Libertad\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'Q' :
               anim_manual = false;
               modoMenu = NADA;
               cout << "MENU PRINCIPAL. Elija una de las siguientes opciones:"<<endl;
               cout << "\tV - Modo Selección de Visualización\n\tD - Modo Selección de Dibujado\n\tM - Modo Animacion Manual\n\tA - Modo Animacion Automático\n\tC - Modo Seleccion de Camaras\n\tQ - Salir" << endl;
               break;
           default:
               if((i-48) >= 0 && (i-48) <= 9){
                   gradoLibertad = (i-48);
                   anim_manual = false;
                   cout << "\t'+' - Aumentar Valor\n\t'-' - Disminuir Valor\n\t[0..9,A,B] - Grado de Libertad\n\tQ - Volver al Menu Principal" << endl;
               }
               else
                   cout << "Tecla sin función en el modo actual" << endl;
               break;
       }
   }
   else if (modoMenu==ANIMAUTO){
       unsigned int i = tecla;
       switch (toupper(tecla)) {
           case '+':
               gallina->aumentarVelocidad(grado);
               cout << "Aumentando velocidad" << endl;
               cout << "\t'+' - Aumentar Velocidad\n\t'-' - Disminuir Velocidad\n\t[1 - 7] - Seleccionar Grado Libertad\n\tG - Cambiar velocidad GENERAL\n\tQ - Volver al Menu Principal" << endl;               break;
           case '-':
               gallina->disminuirVelocidad(grado);
               cout << "Disminuyendo velocidad" << endl;
               cout << "\t'+' - Aumentar Velocidad\n\t'-' - Disminuir Velocidad\n\t[1 - 7] - Seleccionar Grado Libertad\n\tG - Cambiar velocidad GENERAL\n\tQ - Volver al Menu Principal" << endl;               break;
           case 'G':
               grado = 0;
               cout << "Cambiar velocidad general activado" << endl;
               cout << "\t'+' - Aumentar Velocidad\n\t'-' - Disminuir Velocidad\n\t[1 - 7] - Seleccionar Grado Libertad\n\tG - Cambiar velocidad GENERAL\n\tQ - Volver al Menu Principal" << endl;
               break;
           case 'Q' :
               anim_auto = false;
               grado = 0;
               modoMenu = NADA;
               cout << "MENU PRINCIPAL. Elija una de las siguientes opciones:"<<endl;
               cout << "\tV - Modo Selección de Visualización\n\tD - Modo Selección de Dibujado\n\tM - Modo Animacion Manual\n\tA - Modo Animacion Automático\n\tC - Modo Seleccion de Camaras\n\tQ - Salir" << endl;
               break;
           default:
               if((i-48) > 0 && (i-48) <= 7){
                   grado = (i-48);
                   cout << "\t'+' - Aumentar Velocidad\n\t'-' - Disminuir Velocidad\n\t[1 - 7] - Seleccionar Grado Libertad\n\tG - Cambiar velocidad GENERAL\n\tQ - Volver al Menu Principal" << endl;               }
               else
                   cout << "Tecla sin función en el modo actual" << endl;
               break;
       }
   }
   else if(modoMenu==CAMARAS){
       switch (toupper(tecla)) {
           case '0':
               camaraActiva = 0;
               change_projection();
               change_observer();
               break;
           case '1':
               camaraActiva = 1;
               change_projection();
               change_observer();
               break;
           case '2':
               camaraActiva = 2;
               change_projection();
               change_observer();
               break;
           case '+':
               zoomCamaraActiva(toupper(tecla));
               break;
           case '-':
               zoomCamaraActiva(toupper(tecla));
               break;
           case 'Q':
               modoMenu = NADA;
               cout << "MENU PRINCIPAL. Elija una de las siguientes opciones:"<<endl;
               cout << "\tV - Modo Selección de Visualización\n\tD - Modo Selección de Dibujado\n\tM - Modo Animacion Manual\n\tA - Modo Animacion Automático\n\tC - Modo Seleccion de Camaras\n\tQ - Salir" << endl;
               break;
           default:
               cout << "Tecla sin función en el modo actual" << endl;
               break;
       }
   }
   return salir;
}
//**************************************************************************

void Escena::teclaEspecial( int Tecla1, int x, int y )
{
   switch ( Tecla1 )
   {
	   case GLUT_KEY_LEFT:
         Observer_angle_y--;
         break;
	   case GLUT_KEY_RIGHT:
         Observer_angle_y++;
         break;
	   case GLUT_KEY_UP:
         Observer_angle_x--;
         break;
	   case GLUT_KEY_DOWN:
         Observer_angle_x++;
         break;
	   case GLUT_KEY_PAGE_UP:

         break;
	   case GLUT_KEY_PAGE_DOWN:

         break;
	}

	//std::cout << Observer_distance << std::endl;
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//
// ratio_xy : relacción de aspecto del viewport ( == ancho(X) / alto(Y) )
//
//***************************************************************************

void Escena::change_projection()
{
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   camaras[camaraActiva].setProyeccion();
}
//**************************************************************************
// Funcion que se invoca cuando cambia el tamaño de la ventana
//***************************************************************************

void Escena::redimensionar( int newWidth, int newHeight )
{
   const float ratio = (float)newWidth/(float)newHeight;
   for(int i = 0; i < camaras.size();i++){
       camaras[i].setLeft(camaras[i].getBottom()*ratio);
       camaras[i].setRight(camaras[i].getTop()*ratio);
   }
   change_projection();
   glViewport( 0, 0, newWidth, newHeight );
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void Escena::change_observer()
{
   // posicion del observador
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   camaras[camaraActiva].setObserver();

}

void Escena::ratonMovido(int x, int y) {
    if(estadoRaton == MOVIENDO_CAMARA_FIRSTPERSON) {
        camaras[camaraActiva].girar(x-xant,y-yant);
        xant = x;
        yant = y;
    }else if (estadoRaton == MOVIENDO_CAMARA_EXAMINAR){
        camaras[camaraActiva].girarExaminar(x-xant, y-yant);
        xant = x;
        yant = y;
    }
}

void Escena::clickRaton ( int boton , int estado , int x, int y ) {
    if (boton == GLUT_RIGHT_BUTTON){
        xant = x;
        yant = y;
        if (estado == GLUT_DOWN) {
            if(camaras[camaraActiva].getSeleccion())
                estadoRaton = MOVIENDO_CAMARA_EXAMINAR;
            else
                estadoRaton = MOVIENDO_CAMARA_FIRSTPERSON;
        } else
            estadoRaton = NO_MOVIMIENTO;
    }else if (boton == GLUT_LEFT_BUTTON) {
        if (estado == GLUT_UP) {
            dibujaSeleccion();
            procesamientoPick(x,y);
        }
    }
}

void Escena::zoomCamaraActiva(char c){
        if(c == '+')
            camaras[camaraActiva].zoom(0.8);
        else if (c == '-')
            camaras[camaraActiva].zoom(1.2);
        change_projection();
    }

void Escena::dibujaSeleccion(){

    // deshabilitamos el degradado
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Limpiar la pantalla

    change_observer();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    dibujar_encapsulado(6);


    glEnable(GL_DITHER);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void Escena::procesamientoPick(int x,int y){
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    GLint viewport[4];
    GLfloat pixel[3];

    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadPixels (x,viewport[3]-y,1,1,GL_RGB,GL_FLOAT,(void *) pixel);

    Tupla3f pixelLeido = {pixel[0],pixel[1],pixel[2]};

    huevo->colorDefecto();
    lata->colorDefecto();
    tapa_inf_lata->colorDefecto();
    tapa_sup_lata->colorDefecto();
    sol->colorDefecto();

    huevo->materialDefecto();
    lata->materialDefecto();
    tapa_inf_lata->materialDefecto();
    tapa_sup_lata->materialDefecto();
    sol->materialDefecto();


    if(pixelLeido(X) == cSel[0](X) && pixelLeido(Y) == cSel[0](Y) && pixelLeido(Z) == cSel[0](Z)){
        //HUEVO
        camaras[camaraActiva].setSeleccion(true);
        camaras[camaraActiva].setAt({-100+mov3*2, -mov3, 50+mov4});
        huevo->setColorEsp({0.8,0.8,0});
        huevo->setMaterialEsp(obsidiana);
    }
    else if(pixelLeido(X) == cSel[1](X) && pixelLeido(Y) == cSel[1](Y) && pixelLeido(Z) == cSel[1](Z)){
        //LATA
        camaras[camaraActiva].setSeleccion(true);
        camaras[camaraActiva].setAt({-150, -100, 50});
        lata->setColorEsp({0.8,0.8,0});
        tapa_inf_lata->setColorEsp({0.8,0.8,0});
        tapa_sup_lata->setColorEsp({0.8,0.8,0});
        lata->setMaterialEsp(obsidiana);
        tapa_inf_lata->setMaterialEsp(obsidiana);
        tapa_sup_lata->setMaterialEsp(obsidiana);
    }
    else if(pixelLeido(X) == cSel[2](X) && pixelLeido(Y) == cSel[2](Y) && pixelLeido(Z) == cSel[2](Z)){
        //SOL
        camaras[camaraActiva].setSeleccion(true);
        camaras[camaraActiva].setAt({-100, 150, 50});
        sol->setColorEsp({0.8,0.8,0});
        sol->setMaterialEsp(obsidiana);
    }
    else{
        camaras[camaraActiva].setSeleccion(false);
        camaras[camaraActiva].setEye(camaras[camaraActiva].getEyeDefecto());
        camaras[camaraActiva].setAt(camaras[camaraActiva].getAtDefecto());
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
}



