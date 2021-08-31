//
// Created by raul on 23/12/20.
//

#include "Camara.h"

Camara::Camara(Tupla3f eye, Tupla3f at, Tupla3f up, int tipo, float left, float right, float near, float far, float top,
               float bottom) {
    this->eye = eye;
    this->eyeDefecto = eye;
    this->at = at;
    this->atDefecto = at;
    this->up = up;
    this->tipo = tipo;
    this->left = left;
    this->right = right;
    this->near = near;
    this->far = far;
    this->top = top;
    this->bottom = bottom;
    objetoSeleccionado = false;
}

void Camara::rotarXExaminar(float angle){
    angle = angle*M_PI/180;
    Tupla3f e = eye - at;
    float mod = sqrt(e.lengthSq());

    e(Y)= e(Y)*cos(angle) + (-sin(angle)*e(Z));
    e(Z)= e(Y)*sin(angle) + cos(angle)*e(Z);

    e = e.normalized() * mod;
    eye = e + at;
}

void Camara::rotarYExaminar(float angle){
    angle = angle*M_PI/180;
    Tupla3f e = eye - at;
    float mod = sqrt(e.lengthSq());

    e(X)= e(X)*cos(angle) + sin(angle)*e(Z);
    e(Z)= e(X)*(-sin(angle))+ cos(angle)*e(Z);

    e = e.normalized() * mod;
    eye = e + at;
}

void Camara::rotarZExaminar(float angle){
    angle = angle*M_PI/180;
    Tupla3f e = eye - at;
    float mod = sqrt(e.lengthSq());

    e(Y)=(e(X)*cos(angle))+(sin(angle)*e(Y));
    e(Z)=(e(X)*(-sin(angle)))+(cos(angle)*e(Y));

    e = e.normalized() * mod;
    eye = e + at;
}

void Camara::rotarXFirstPerson(float angle){
    at(X)=at(X)+angle;
    //eye(Y)=eye(Y)+angle;
}
void Camara::rotarYFirstPerson(float angle){
    at(Y)=at(Y)+angle;
    //eye(X)=eye(X)+angle;
}
void Camara::rotarZFirstPerson(float angle){
    at(Z)=at(Z)+angle;
    //eye(Z)=eye(Z)+angle;
}

void Camara::zoom(float factor){
    this->left *= factor;
    this->right *= factor;
    this->top *= factor;
    this->bottom *= factor;
    this->setProyeccion();
}

void Camara::setObserver() {
    gluLookAt(eye(X),eye(Y),eye(Z),at(X),at(Y),at(Z),up(X),up(Y),up(Z));
}

void Camara::setProyeccion(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(tipo==0)
        glFrustum(left, right, bottom, top, near, far);
    else
        glOrtho(left, right, bottom, top, near, far);
}

void Camara::girar(int x, int y) {
    rotarXFirstPerson(x);
    rotarYFirstPerson(y);
}

void Camara::girarExaminar(int x, int y) {
    rotarXExaminar(y);
    rotarYExaminar(x);
}

void Camara::setEye(const Tupla3f & eye) {
    this->eye = eye;
}

void Camara::setAt(const Tupla3f & at) {
    this->at = at;
}

void Camara::setRight(const float &  r) {
    this->right = r;
}

void Camara::setLeft(const float &  l) {
    this->left = l;
}

void Camara::setTop(const float &  t) {
    this->top = t;
}

void Camara::setBottom(const float &  b) {
    this->bottom = b;
}

void Camara::setSeleccion(const bool & o) {
    this->objetoSeleccionado = o;
}

float Camara::getTop() {
    return this->top;
}

float Camara::getBottom() {
    return this->bottom;
}

bool Camara::getSeleccion() {
    return this->objetoSeleccionado;
}

Tupla3f Camara::getEyeDefecto() {
    return this->eyeDefecto;
}

Tupla3f Camara::getAtDefecto() {
    return this->atDefecto;
}