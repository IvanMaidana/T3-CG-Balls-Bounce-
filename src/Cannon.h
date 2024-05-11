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
    // Atualiza o ponto inicial do canhão
    this->point1.set(pointIni.x, pointIni.y);
    // Calcula o vetor direção do canhão em relação ao mouse
    Vector2 direction = coordMouse - this->point1;
    direction.normalize();
    // Define o tamanho do vetor direção
    float length = 50.0; // Tamanho desejado do canhão
    // Calcula o ponto final do canhão usando o vetor direção e o tamanho
    this->point2 = this->point1 + direction * length;
    // Desenha o canhão
    CV::color(this->color);
    CV::line(this->point1, this->point2);
}

};


#endif // CANNON_H_INCLUDED
