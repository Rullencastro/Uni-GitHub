#include "aux.h"
#include "malla.h"

// *****************************************************************************
//
// Clase Malla3D
//
// *****************************************************************************

// Visualización en modo inmediato con 'glDrawElements'

void Malla3D::draw_ModoInmediato()
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
        glDrawElements( GL_TRIANGLES , fp.size ()*3 , GL_UNSIGNED_INT , fp.data() );
        glColorPointer(3, GL_FLOAT,0,cvimpar.data());
        glDrawElements( GL_TRIANGLES , fi.size ()*3 , GL_UNSIGNED_INT , fi.data() );
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
       glDrawElements(GL_TRIANGLES, f.size() * 3 , GL_UNSIGNED_INT, f.data());
   }




   glDisableClientState( GL_VERTEX_ARRAY );
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState( GL_COLOR_ARRAY );
   glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisable(GL_TEXTURE_2D);

}

GLuint Malla3D::CrearVBO (GLuint tipo_vbo, GLuint tamanio_bytes , GLvoid * puntero_ram )
{
   GLuint id_vbo;
   glGenBuffers ( 1 , & id_vbo );
      
   glBindBuffer ( tipo_vbo , id_vbo );

   glBufferData ( tipo_vbo , tamanio_bytes , puntero_ram , GL_STATIC_DRAW );

   glBindBuffer ( tipo_vbo , 0 );
   return id_vbo ;
}




// -----------------------------------------------------------------------------
// Visualización en modo diferido con 'glDrawElements' (usando VBOs)

void Malla3D::draw_ModoDiferido()
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
   
   //especificar localización y formato de la tabla de vértices, habilitar tabla
   glBindBuffer ( GL_ARRAY_BUFFER , id_vbo_ver );  // activar VBO de vértices

   glVertexPointer ( 3 , GL_FLOAT , 0 , 0 );       // especifica formato y off-set (=0)

   glBindBuffer ( GL_ARRAY_BUFFER , 0 );           // desactivar VBO de vértices.

   glEnableClientState ( GL_VERTEX_ARRAY );        // habilitar tabla de vértices

    if(modo_activo == 0)
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colp);
    else if(modo_activo == 1)
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_coll);
    else if(modo_activo == 2)
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_cols);
    else if(modo_activo == 6) {
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colSel);
        textura = nullptr;
    }
    else if(modo_activo == 3) {
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colpar);
        glColorPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , id_vbo_tripar ); // activar VBO   de triángulos
        glDrawElements ( GL_TRIANGLES , 3* fp . size () , GL_UNSIGNED_INT , 0 ) ;
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

        glEnableClientState(GL_COLOR_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_colimpar);
        glColorPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , id_vbo_triimpar ); // activar VBO   de triángulos
        glDrawElements ( GL_TRIANGLES , 3* fi . size () , GL_UNSIGNED_INT , 0 ) ;
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

        glEnableClientState(GL_COLOR_ARRAY);
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

   glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , id_vbo_tri );
   glDrawElements ( GL_TRIANGLES , 3 * f.size () , GL_UNSIGNED_INT , 0 ) ;
   glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

   glDisableClientState ( GL_VERTEX_ARRAY );
    glDisableClientState ( GL_NORMAL_ARRAY );
    glDisableClientState ( GL_COLOR_ARRAY );
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}
// -----------------------------------------------------------------------------
// Función de visualización de la malla,

void Malla3D::draw(unsigned int mode,unsigned int color)
{
   modo_activo = color;

   if(mode == 1)
      draw_ModoInmediato();
   if(mode == 2)
      draw_ModoDiferido();

}

void Malla3D::calcular_normales() {
    Tupla3f p,q,r,a,b,mc,nc;
    float mod_mc,mod_mv;
    nv.resize(v.size());
    for(int i = 0; i < nv.size() ; i++){
        nv[i] = {0,0,0};
    }
    for(int i = 0; i < f.size(); i++){
        p = v[f[i](X)];
        q = v[f[i](Y)];
        r = v[f[i](Z)];
        a = q - p;
        b = r - p;
        mc = a.cross(b);
        //mod_mc = sqrtf(pow(mc(X),2)+pow(mc(Y),2) + pow(mc(Z),2));
        //nc = {mc(X)/mod_mc , mc(Y)/mod_mc , mc(Z)/mod_mc};
        nc = mc.normalized();
        nv[f[i](X)] = nv[f[i](X)] + nc;
        nv[f[i](Y)] = nv[f[i](Y)] + nc;
        nv[f[i](Z)] = nv[f[i](Z)] + nc;
    }
    for(int i = 0; i < nv.size(); i++){
        //mod_mv = sqrtf(pow(nv[i](X),2)+pow(nv[i](Y),2) + pow(nv[i](Z),2));
        //nv[i] = {nv[i](X)/mod_mv , nv[i](Y)/mod_mv , nv[i](Z)/mod_mv};
        nv[i] = nv[i].normalized();
    }
}

void Malla3D::inicializarColores() {
    Tupla3f c1({1,0,0}), c2({0,1,0}) , c3({0,0,1}),c4({0,0,0}),c5({0.5,0,0.5});
    for (int i = 0; i < v.size(); i++) {
        cvpar.push_back(c1);
        cvimpar.push_back(c2);
        cvp.push_back(c5);
        cvl.push_back(c4);
        cvs.push_back(c3);
        cvSel.push_back(c3);
    }
    c_aux = c3;
}

void Malla3D::setMaterial(Material mat){
    m.setMaterial(mat);
    m_aux.setMaterial(mat);
}

void Malla3D::setTextura(std::string archivo) {
    textura = new Textura(archivo);
    textur = archivo;
}

void Malla3D::cambiarColor(Tupla3f c) {
    for (int i = 0; i < v.size(); i++) {
        cvs.pop_back();
    }
    for (int i = 0; i < v.size(); i++) {
        cvs.push_back(c);
    }
    c_aux = c;
}

void Malla3D::cambiarColorSel(Tupla3f c) {
    for (int i = 0; i < v.size(); i++) {
        cvSel.pop_back();
    }
    for (int i = 0; i < v.size(); i++) {
        cvSel.push_back(c);
    }
}

void Malla3D::setColorEsp(Tupla3f c) {
    for (int i = 0; i < v.size(); i++) {
        cvs.pop_back();
    }
    for (int i = 0; i < v.size(); i++) {
        cvs.push_back(c);
    }
}

void Malla3D::setMaterialEsp(Material mat) {
    m.setMaterial(mat);
}

void Malla3D::colorDefecto() {
    cambiarColor(c_aux);
}

void Malla3D::materialDefecto() {
    m.setMaterial(m_aux);
}
