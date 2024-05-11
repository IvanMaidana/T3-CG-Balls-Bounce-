#ifndef CANNON_H_INCLUDED
#define CANNON_H_INCLUDED

#include "gl_canvas2d.h"
#include "Vector2.h"

class Cannon{
    Vector2 point1, point2;
    int color;


    public:

    Cannon(){
        this->point1.set(0, 0);
        this->point2.set(1, 1);
        this->color = 0;
    }

    Vector2 getPoint2(){
        return this->point2;
    }

    Vector2 getPoint1(){
        return this->point1;
    }

    void draw(Vector2 coordMouse, Vector2 pointIni){
    // Atualiza o ponto inicial do canh�o
    this->point1.set(pointIni.x, pointIni.y);
    // Calcula o vetor dire��o do canh�o em rela��o ao mouse
    Vector2 direction = coordMouse - this->point1;
    direction.normalize();
    // Define o tamanho do vetor dire��o
    float length = 50.0; // Tamanho desejado do canh�o
    // Calcula o ponto final do canh�o usando o vetor dire��o e o tamanho
    this->point2 = this->point1 + direction * length;
    // Desenha o canh�o
    CV::color(this->color);
    CV::line(this->point1, this->point2);
}

};


#endif // CANNON_H_INCLUDED
