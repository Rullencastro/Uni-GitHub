#include "aux.h"
#include "objrevolucion.h"

// *****************************************************************************
//
// Clase ObjRevolucion (práctica 2)
//
// *****************************************************************************


// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

ObjRevolucion::ObjRevolucion() {}

ObjRevolucion::ObjRevolucion(const std::string & archivo, int num_instancias, bool tapa_sup, bool tapa_inf,char eje, bool textura) {
   // completar ......(práctica 2)
   std::vector<Tupla3f> perfil_original;
   ply::read_vertices(archivo,perfil_original);
   ts = tapa_sup;
   ti = tapa_inf;

   switch (toupper(eje)) {
       case 'Y':
       if(perfil_original[0](Y) > perfil_original[perfil_original.size()-1](Y))
           std::reverse(perfil_original.begin(),perfil_original.end());
       if (tapa_inf) {
           if (fabs(perfil_original[0](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z))< std::numeric_limits<float>::epsilon())
               tapaInferior = perfil_original[0];
           else {
               tapaInferior(X) = 0;
               tapaInferior(Y) = perfil_original[0](Y) ;
               tapaInferior(Z) = 0;
           }
       }
       if (tapa_sup) {
           if (fabs(perfil_original[perfil_original.size() - 1](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z))< std::numeric_limits<float>::epsilon())
               tapaSuperior = perfil_original[perfil_original.size() - 1];
           else {
               tapaSuperior(X) = 0;
               tapaSuperior(Y) = perfil_original[perfil_original.size() - 1](Y) ;
               tapaSuperior(Z) = 0;
           }
       }
       break;

       case 'X':
           if(perfil_original[0](X) > perfil_original[perfil_original.size()-1](X))
               std::reverse(perfil_original.begin(),perfil_original.end());
           if (tapa_inf) {
               if (fabs(perfil_original[0](Y))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z))< std::numeric_limits<float>::epsilon())
                   tapaInferior = perfil_original[0];
               else {
                   tapaInferior(X) = perfil_original[0](X) ;
                   tapaInferior(Y) = 0;
                   tapaInferior(Z) = 0;
               }
           }
           if (tapa_sup) {
               if (fabs(perfil_original[perfil_original.size() - 1](Y))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z))< std::numeric_limits<float>::epsilon())
                   tapaSuperior = perfil_original[perfil_original.size() - 1];
               else {
                   tapaSuperior(X) = perfil_original[perfil_original.size() - 1](X) ;
                   tapaSuperior(Y) = 0;
                   tapaSuperior(Z) = 0;
               }
           }
           break;

       case 'Z':
           if(perfil_original[0](Z) > perfil_original[perfil_original.size()-1](Z))
               std::reverse(perfil_original.begin(),perfil_original.end());
           if (tapa_inf) {
               if (fabs(perfil_original[0](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Y))< std::numeric_limits<float>::epsilon())
                   tapaInferior = perfil_original[0];
               else {
                   tapaInferior(X) = 0;
                   tapaInferior(Y) = 0;
                   tapaInferior(Z) = perfil_original[0](Z) ;
               }
           }

           if (tapa_sup) {
               if (fabs(perfil_original[perfil_original.size() - 1](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Y))< std::numeric_limits<float>::epsilon())
                   tapaSuperior = perfil_original[perfil_original.size() - 1];
               else {
                   tapaSuperior(X) = 0;
                   tapaSuperior(Y) = 0;
                   tapaSuperior(Z) = perfil_original[perfil_original.size() - 1](Z) ;
               }
           }
           break;
   }

    if(tapa_inf || tapa_sup) {
        std::vector <Tupla3f> perfil_sin_tapas = perfil_original;
        switch (toupper(eje)) {
            case 'Y':
                if (fabs(perfil_original[0](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.begin());
                if (fabs(perfil_original[perfil_original.size() - 1](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.end() - 1);
                crearMalla(perfil_sin_tapas, num_instancias, true, eje,textura);
                break;
            case 'X':
                if (fabs(perfil_original[0](Y)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.begin());
                if (fabs(perfil_original[perfil_original.size() - 1](Y)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.end() - 1);
                crearMalla(perfil_sin_tapas, num_instancias, true, eje,textura);
                break;
            case 'Z':
                if (fabs(perfil_original[0](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Y)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.begin());
                if (fabs(perfil_original[perfil_original.size() - 1](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Y)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.end() - 1);
                crearMalla(perfil_sin_tapas, num_instancias, true, eje,textura);
                break;
        }
    }
    else
        crearMalla(perfil_original,num_instancias,false,eje,textura);

    calcular_normales();


    for(int i = 0; i < f.size(); i++){
        if(i > num_instancias && ti && !ts)
            ft.push_back(f[i]);
        else if(i > num_instancias && !ti && ts)
            ft.push_back(f[i]);
        else if(i > num_instancias*2 && ti && ts)
            ft.push_back(f[i]);
        else
            ft.push_back(f[i]);

        if(i % 2 == 0) {
            fp.push_back(f[i]);
            if(i > num_instancias && tapa_inf && !tapa_sup)
                fpt.push_back(f[i]);
            else if(i > num_instancias && !tapa_inf && tapa_sup)
                fpt.push_back(f[i]);
            else if(i > num_instancias*2 && tapa_inf && tapa_sup)
                fpt.push_back(f[i]);
        }
        else {
            fi.push_back(f[i]);
            if(i > num_instancias && tapa_inf && !tapa_sup)
                fit.push_back(f[i]);
            else if(i > num_instancias && !tapa_inf && tapa_sup)
                fit.push_back(f[i]);
            else if(i > num_instancias*2 && tapa_inf && tapa_sup)
                fit.push_back(f[i]);
        }

    }


    inicializarColores();

    if(textura)
        asignarTexCoord(num_instancias,perfil_original.size());
}

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un vector de puntos)

 
ObjRevolucion::ObjRevolucion(std::vector<Tupla3f> archivo, int num_instancias, bool tapa_sup, bool tapa_inf,char eje,bool textura) {
    std::vector<Tupla3f> perfil_original = archivo;
    ts = tapa_sup;
    ti = tapa_inf;

    if(perfil_original[0](Y) > perfil_original[perfil_original.size()-1](Y))
        std::reverse(perfil_original.begin(),perfil_original.end());

    switch (toupper(eje)) {
        case 'Y':
            if (tapa_inf) {
                if (fabs(perfil_original[0](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z))< std::numeric_limits<float>::epsilon())
                    tapaInferior = perfil_original[0];
                else {
                    tapaInferior(X) = 0;
                    tapaInferior(Y) = perfil_original[0](Y) ;
                    tapaInferior(Z) = 0;
                }
            }

            if (tapa_sup) {
                if (fabs(perfil_original[perfil_original.size() - 1](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z))< std::numeric_limits<float>::epsilon())
                    tapaSuperior = perfil_original[perfil_original.size() - 1];
                else {
                    tapaSuperior(X) = 0;
                    tapaSuperior(Y) = perfil_original[perfil_original.size() - 1](Y) ;
                    tapaSuperior(Z) = 0;
                }
            }
            break;
        case 'X':
            if (tapa_inf) {
                if (fabs(perfil_original[0](Y))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z))< std::numeric_limits<float>::epsilon())
                    tapaInferior = perfil_original[0];
                else {
                    tapaInferior(X) = perfil_original[0](X) ;
                    tapaInferior(Y) = 0;
                    tapaInferior(Z) = 0;
                }
            }

            if (tapa_sup) {
                if (fabs(perfil_original[perfil_original.size() - 1](Y))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z))< std::numeric_limits<float>::epsilon())
                    tapaSuperior = perfil_original[perfil_original.size() - 1];
                else {
                    tapaSuperior(X) = perfil_original[perfil_original.size() - 1](X) ;
                    tapaSuperior(Y) = 0;
                    tapaSuperior(Z) = 0;
                }
            }
            break;
        case 'Z':
            if (tapa_inf) {
                if (fabs(perfil_original[0](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Y))< std::numeric_limits<float>::epsilon())
                    tapaInferior = perfil_original[0];
                else {
                    tapaInferior(X) = 0;
                    tapaInferior(Y) = 0;
                    tapaInferior(Z) = perfil_original[0](Z) ;
                }
            }

            if (tapa_sup) {
                if (fabs(perfil_original[perfil_original.size() - 1](X))< std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Y))< std::numeric_limits<float>::epsilon())
                    tapaSuperior = perfil_original[perfil_original.size() - 1];
                else {
                    tapaSuperior(X) = 0;
                    tapaSuperior(Y) = 0;
                    tapaSuperior(Z) = perfil_original[perfil_original.size() - 1](Z) ;
                }
            }
            break;
    }

    if(tapa_inf || tapa_sup) {
        std::vector <Tupla3f> perfil_sin_tapas = perfil_original;
        switch (toupper(eje)) {
            case 'Y':
                if (fabs(perfil_original[0](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.begin());
                if (fabs(perfil_original[perfil_original.size() - 1](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.end() - 1);
                crearMalla(perfil_sin_tapas, num_instancias, true, eje,textura);
                break;
            case 'X':
                if (fabs(perfil_original[0](Y)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.begin());
                if (fabs(perfil_original[perfil_original.size() - 1](Y)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Z)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.end() - 1);
                crearMalla(perfil_sin_tapas, num_instancias, true, eje,textura);
                break;
            case 'Z':
                if (fabs(perfil_original[0](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[0](Y)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.begin());
                if (fabs(perfil_original[perfil_original.size() - 1](X)) < std::numeric_limits<float>::epsilon() && fabs(perfil_original[perfil_original.size() - 1](Y)) < std::numeric_limits<float>::epsilon())
                    perfil_sin_tapas.erase(perfil_sin_tapas.end() - 1);
                crearMalla(perfil_sin_tapas, num_instancias, true, eje,textura);
                break;
        }
    }
    else
        crearMalla(perfil_original,num_instancias,false,eje,textura);

    calcular_normales();

    for(int i = 0; i < f.size(); i++){
        if(i > num_instancias && ti && !ts)
            ft.push_back(f[i]);
        else if(i > num_instancias && !ti && ts)
            ft.push_back(f[i]);
        else if(i > num_instancias*2 && ti && ts)
            ft.push_back(f[i]);
        else
            ft.push_back(f[i]);

        if(i % 2 == 0) {
            fp.push_back(f[i]);
            if(i > num_instancias && tapa_inf && !tapa_sup)
                fpt.push_back(f[i]);
            else if(i > num_instancias && !tapa_inf && tapa_sup)
                fpt.push_back(f[i]);
            else if(i > num_instancias*2 && tapa_inf && tapa_sup)
                fpt.push_back(f[i]);
        }
        else {
            fi.push_back(f[i]);
            if(i > num_instancias && tapa_inf && !tapa_sup)
                fit.push_back(f[i]);
            else if(i > num_instancias && !tapa_inf && tapa_sup)
                fit.push_back(f[i]);
            else if(i > num_instancias*2 && tapa_inf && tapa_sup)
                fit.push_back(f[i]);
        }
    }

    inicializarColores();

    asignarTexCoord(num_instancias,perfil_original.size());
}

void ObjRevolucion::crearMalla(std::vector<Tupla3f> perfil_original, int num_instancias,bool conTapas,char eje,bool textura) {
    Tupla3f nuevo_vertice;
    const int N = num_instancias;
    const int M = perfil_original.size();

    const float PI = 3.141592653589793238462;
    float angulo;
    v.clear();
    switch (toupper(eje)) {
        case 'Y':
            for (int i = 0; i <= N - 1; ++i) {
                angulo = (2.0 * PI) / N;
                angulo = angulo * i;
                for (int j = 0; j <= M - 1; ++j) {
                    nuevo_vertice(X) = perfil_original[j](X) * cos(angulo) + perfil_original[j](Z) * sin(angulo);
                    nuevo_vertice(Y) =  perfil_original[j](Y);
                    nuevo_vertice(Z) =  perfil_original[j](X) * (-sin(angulo)) + perfil_original[j](Z) * cos(angulo);

                    v.push_back(nuevo_vertice);
                }
            }
        break;
        case 'X':
            for (int i = 0; i <= N - 1; ++i) {
                angulo = (2.0 * PI) / N;
                angulo = angulo * i;
                for (int j = 0; j <= M - 1; ++j) {
                    nuevo_vertice(X) = perfil_original[j](X);
                    nuevo_vertice(Y) = perfil_original[j](Y) * cos(angulo) + perfil_original[j](Z) * (-sin(angulo));
                    nuevo_vertice(Z) = perfil_original[j](Y) * sin(angulo) + perfil_original[j](Z) * cos(angulo);

                    v.push_back(nuevo_vertice);
                }
            }
        break;
        case 'Z':
            for (int i = 0; i <= N - 1; ++i) {
                angulo = (2.0 * PI) / N;
                angulo = angulo * i;
                for (int j = 0; j <= M - 1; ++j) {
                    nuevo_vertice(X) = perfil_original[j](X) * cos(angulo) + perfil_original[j](Y) * (-sin(angulo));
                    nuevo_vertice(Y) = perfil_original[j](X) * sin(angulo) + perfil_original[j](Y) * cos(angulo);
                    nuevo_vertice(Z) = perfil_original[j](Z);

                    v.push_back(nuevo_vertice);
                }
            }
        break;
    }

    if(textura){
        for(int i = 0; i < perfil_original.size();i++)
            v.push_back(perfil_original[i]);
    }

    f.clear();
    int a,b;

    if(conTapas) {

        if (ti){
            v.push_back(tapaInferior);
            for (int i = 0; i < N; ++i) {
                a = M * i;
                b = M * ((i + 1) % N);
                f.push_back({M*N, b, a});
            }
        }
        if (ts) {
            v.push_back(tapaSuperior);
            for (int i = 0; i < N; ++i) {
                a = M * (i + 1) - 1;
                b = M * (((i + 1) % N)+1)- 1;
                f.push_back({M * N + 1, a, b});
            }
        }
    }


    if(!textura) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M - 1; ++j) {
                a = M * i + j;
                b = M * ((i + 1) % N) + j;
                f.push_back({a, b, b + 1});
                f.push_back({a, b + 1, a + 1});
            }
        }
    }
    if(textura) {
        for (int i = 0; i < N - 1; ++i) {
            for (int j = 0; j < M - 1; ++j) {
                a = M * i + j;
                b = M * ((i + 1) % N) + j;
                f.push_back({a, b, b + 1});
                f.push_back({a, b + 1, a + 1});
            }
        }

        for (int j = 0; j < M - 1; ++j) {
            a = M * (N - 1) + j;
            b = M * N + j;
            f.push_back({a, b, b + 1});
            f.push_back({a, b + 1, a + 1});
        }
    }
}


void ObjRevolucion::draw_ModoInmediato_Revolucion(bool conTapas)
{
    // habilitar uso de un array de vértices
    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 3 , GL_FLOAT , 0 , v.data () ) ;

    glEnableClientState( GL_COLOR_ARRAY);
    if (modo_activo == 0){
        glColorPointer(3, GL_FLOAT,0,cvp.data());
    }
    else if (modo_activo == 1){
        glColorPointer(3, GL_FLOAT,0,cvl.data());
    }
    else if (modo_activo == 2){
        glColorPointer(3, GL_FLOAT,0,cvs.data());
    }
    else if (modo_activo == 6){
        glColorPointer(3, GL_FLOAT,0,cvSel.data());
        textura = nullptr;
    }
    else if (modo_activo == 3){
        glColorPointer(3, GL_FLOAT,0,cvpar.data());
        if(conTapas)
            glDrawElements( GL_TRIANGLES , fp.size ()*3 , GL_UNSIGNED_INT , fp.data());
        else
            glDrawElements( GL_TRIANGLES , fpt.size ()*3 , GL_UNSIGNED_INT , fpt.data());
        glColorPointer(3, GL_FLOAT,0,cvimpar.data());
        if(conTapas)
            glDrawElements( GL_TRIANGLES , fi.size ()*3 , GL_UNSIGNED_INT , fi.data());
        else
            glDrawElements( GL_TRIANGLES , fit.size ()*3 , GL_UNSIGNED_INT , fit.data());
    }
    else if (modo_activo == 4 || modo_activo == 5){
        m.aplicar();
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, nv.data());
    }

    if(textura != nullptr) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, ct.data());
        textura->activar();
    }

    if(textura == nullptr && textur != "NINGUNA")
        setTextura(textur);

    if(modo_activo != 3) {
        if(conTapas)
            glDrawElements(GL_TRIANGLES, f.size() * 3 , GL_UNSIGNED_INT, f.data());
        else
            glDrawElements(GL_TRIANGLES, ft.size() * 3 , GL_UNSIGNED_INT, ft.data());
    }

    // deshabilitar array de vértices
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState ( GL_NORMAL_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

}

void ObjRevolucion::draw_ModoDiferido_Revolucion(bool conTapas)
{
    // (la primera vez, se deben crear los VBOs y guardar sus identificadores en el objeto)

    if(id_vbo_ver == 0)
        id_vbo_ver = CrearVBO(GL_ARRAY_BUFFER, v.size()*sizeof(Tupla3f) ,v.data());
    if(id_vbo_tri == 0)
        id_vbo_tri = CrearVBO(GL_ELEMENT_ARRAY_BUFFER,  f.size()*sizeof(Tupla3i) , f.data());
    if(id_vbo_tripar == 0)
        id_vbo_tripar = CrearVBO(GL_ELEMENT_ARRAY_BUFFER,  fp.size()*sizeof(Tupla3i) , fp.data());
    if(id_vbo_triimpar == 0)
        id_vbo_triimpar = CrearVBO(GL_ELEMENT_ARRAY_BUFFER,  fi.size()*sizeof(Tupla3i) , fi.data());
    if(id_vbo_tri_tapas == 0)
        id_vbo_tri_tapas = CrearVBO(GL_ELEMENT_ARRAY_BUFFER,  ft.size()*sizeof(Tupla3i) , ft.data());
    if(id_vbo_tripar_tapas == 0)
        id_vbo_tripar_tapas = CrearVBO(GL_ELEMENT_ARRAY_BUFFER,  fpt.size()*sizeof(Tupla3i) , fpt.data());
    if(id_vbo_triimpar_tapas == 0)
        id_vbo_triimpar_tapas = CrearVBO(GL_ELEMENT_ARRAY_BUFFER,  fit.size()*sizeof(Tupla3i) , fit.data());
    if(id_vbo_colp == 0)
        id_vbo_colp = CrearVBO(GL_ARRAY_BUFFER, cvp.size()*sizeof(Tupla3f) ,cvp.data());
    if(id_vbo_coll == 0)
        id_vbo_coll = CrearVBO(GL_ARRAY_BUFFER, cvl.size()*sizeof(Tupla3f) ,cvl.data());
    if(id_vbo_cols == 0)
        id_vbo_cols = CrearVBO(GL_ARRAY_BUFFER, cvs.size()*sizeof(Tupla3f) ,cvs.data());
    if(id_vbo_colpar == 0)
        id_vbo_colpar = CrearVBO(GL_ARRAY_BUFFER, cvpar.size()*sizeof(Tupla3f) ,cvpar.data());
    if(id_vbo_colimpar == 0)
        id_vbo_colimpar = CrearVBO(GL_ARRAY_BUFFER, cvimpar.size()*sizeof(Tupla3f) ,cvimpar.data());
    if(id_vbo_nor == 0)
        id_vbo_nor = CrearVBO(GL_ARRAY_BUFFER, nv.size()*sizeof(Tupla3f) ,nv.data());
    if(id_vbo_tex == 0)
        id_vbo_tex = CrearVBO(GL_ARRAY_BUFFER, ct.size()*sizeof(Tupla2f) ,ct.data());
    if(id_vbo_colSel == 0)
        id_vbo_colSel = CrearVBO(GL_ARRAY_BUFFER, cvSel.size()*sizeof(Tupla3f) ,cvSel.data());

    // especificar localización y formato de la tabla de vértices, habilitar tabla
    glBindBuffer ( GL_ARRAY_BUFFER , id_vbo_ver );  // activar VBO de vértices

    glVertexPointer ( 3 , GL_FLOAT , 0 , 0 );       // especifica formato y off-set (=0)

    glBindBuffer ( GL_ARRAY_BUFFER , 0 );           // desactivar VBO de vértices.

    glEnableClientState ( GL_VERTEX_ARRAY );        // habilitar tabla de vértices
    glEnableClientState(GL_COLOR_ARRAY);

    if(modo_activo == 0)
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colp);
    else if(modo_activo == 1)
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_coll);
    else if(modo_activo == 2)
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_cols);
    else if(modo_activo == 6){
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colSel);
        textura = nullptr;
    }
    else if(modo_activo == 3) {
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colpar);
        glColorPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if(conTapas) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tripar);
            glDrawElements(GL_TRIANGLES, 3 * fp.size(), GL_UNSIGNED_INT, 0);
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tripar_tapas);
            glDrawElements(GL_TRIANGLES, 3 * fpt.size(), GL_UNSIGNED_INT, 0);
        }
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colimpar);
        glColorPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if(conTapas) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_triimpar);
            glDrawElements(GL_TRIANGLES, 3 * fi.size(), GL_UNSIGNED_INT, 0);
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_triimpar_tapas);
            glDrawElements(GL_TRIANGLES, 3 * fit.size(), GL_UNSIGNED_INT, 0);
        }
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );
    }
    else if (modo_activo == 4 || modo_activo == 5 ){
        m.aplicar();
        glEnableClientState(GL_NORMAL_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_nor);
        glNormalPointer(GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if(modo_activo != 3 && modo_activo != 4 && modo_activo != 5) {
        glColorPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnableClientState(GL_COLOR_ARRAY);
    }

    if(textura != nullptr) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_tex);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        textura->activar();
    }

    if(textura == nullptr && textur != "NINGUNA")
        setTextura(textur);
    // visualizar triángulos con glDrawElements (puntero a tabla == 0)
    if(conTapas) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri); // activar VBO   de triángulos
        glDrawElements(GL_TRIANGLES, 3 * f.size(), GL_UNSIGNED_INT, 0);
    }
    else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri_tapas);
        glDrawElements(GL_TRIANGLES, 3 * ft.size(), GL_UNSIGNED_INT, 0);
    }

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );   // desactivar VBO de triángulos

    //desactivar uso de array de vértices
    glDisableClientState ( GL_VERTEX_ARRAY );
    glDisableClientState ( GL_NORMAL_ARRAY );
    glDisableClientState ( GL_COLOR_ARRAY );
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

void ObjRevolucion::draw(unsigned int mode,unsigned int color,bool conTapas)
{
    modo_activo = color;

    if(mode == 1)
        draw_ModoInmediato_Revolucion(conTapas);
    if(mode == 2)
        draw_ModoDiferido_Revolucion(conTapas);
}

void ObjRevolucion::asignarTexCoord(float N,float M) {
    ct.resize(v.size());
    float s,t;
    std::vector<float> d;
    d.resize(M);
    d[0] = 0;
    for(int i = 1; i < M ; i++){
        Tupla3f aux = v[i] - v[i-1];
        d[i] = d[i-1] + sqrtf(pow(aux(X),2) + pow(aux(Y),2) + pow(aux(Z),2));
    }


    for(int i = 0; i < (N + 1) ;i++ ){
        s = i/N;

        for(int j = 0; j < M ;j++){
            t = d[j]/d[M-1];
            ct[M*i+j] = {s,t};
        }
    }
}

