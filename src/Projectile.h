#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#include "Vector2.h"
#include "gl_canvas2d.h"

class Projectile
{

public:
    Vector2 position, direction;
    float speedBall = 0.01, radius = 3;


    Projectile(Vector2 initialPosition1, Vector2 initialPosition2)
    {
        this->position = initialPosition2;
        this->direction = initialPosition2 - initialPosition1;
    }

    void setDirection(Vector2 _direction){
        direction = _direction;
    }

    Vector2 getDirection(){
        return this->direction;
    }

    Vector2 getPosition(){
        return this->position;
    }

    float getRadius(){
        return this->radius;
    }




    bool Colidiu(Vector2 xy)
    {
        if ( xy.x == position.x || xy.y == position.y)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    void Anima()
    {
        CV::color(0, 1, 0);
        position = position + direction*0.01;
        CV::circleFill(position, radius, 10);
    }

};

#endif // PROJECTILE_H_INCLUDED
