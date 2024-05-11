#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Projectile.h"
#include<math.h>

class Line{

public:
    Vector2 position, direction;
    int color;


    Line(Vector2 point1, Vector2 point2, int color){
        this->position = point1;
        this->direction = point2;
        this->color = color;
    }

    void draw(){
        CV::color(this->color);
        Vector2 pf = position+direction;
        CV::line(this->position, pf);
    }

    Vector2 getDirection(){
        return this->direction;
    }

    Vector2 getPosition(){
        return this->position;
    }

    bool intersects(Projectile projectile, float &s, float &t)
    {
      float a, b, c, r2, delta;
      // � subtraido o centro da circunferencia para centrarmos a circunferencia na origem
      Vector2 centro(position.x - projectile.position.x,  position.y - projectile.position.y);
      // o raio ao quadrado da circunferencia
      r2 = projectile.radius * projectile.radius;
      // a da quadrica
      a = direction.x*direction.x + direction.y*direction.y;
      // b da quadrica
      b = 2*(centro.x*direction.x + centro.y*direction.y);
      // c da quadrica
      c = centro.x*centro.x + centro.y*centro.y - r2;
      // calcula o delta da quadrica
      delta = b*b - 4*a*c;
      // se negativo nao colide em hipotese alguma
      if( delta < 0 )
         return false;
      else
      {
         // calcula os pontos de interseccao
         s = ((-b)+sqrt(delta)) / (2*a);
         t = ((-b)-sqrt(delta)) / (2*a);
         //printf("calculou o ponto de intersequicao \n");
         if( (t > 0 && t < 1) || (s > 0 && s < 1) ){
              return true;
         }
      }
      return false;
   }

};


#endif // LINE_H_INCLUDED
