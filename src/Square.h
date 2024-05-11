#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

#include <string>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Line.h"

class Square {

public:
    Line* segments[4]; // Array de quatro segmentos de reta
    int life; // Variável inteira para representar a vida do quadrado
    Vector2 coordLife;

    // Construtor que inicializa os segmentos de reta e a vida do quadrado
    Square(Vector2 topLeft, float width, float height, int life) {

        // Inicializa os quatro segmentos de reta que formam o quadrado
        segments[0] = new Line(topLeft, Vector2(width, 0), 9); // Top
        segments[1] = new Line(Vector2(topLeft.x + width, topLeft.y), Vector2(0, -height), 9); // Right
        segments[2] = new Line(Vector2(topLeft.x, topLeft.y - height), Vector2(width, 0), 1); // Bottom
        segments[3] = new Line(topLeft , Vector2(0, -height), 1); // Left

        // Define a vida do quadrado
        this->life = life;
        this->coordLife.set(topLeft.x + width/2, topLeft.y - height/2);
    }

    // Método para desenhar o quadrado na tela
    void draw() {
        for (int i = 0; i < 4; i++) {
            segments[i]->draw();
        }
        CV::color(1);
        std::string lifeString = std::to_string(life);
        CV::text(coordLife.x, coordLife.y , lifeString.c_str());
    }

    // Getter para obter a vida do quadrado
    int getLife() const {
        return life;
    }

    // Setter para definir a vida do quadrado
    void setLife(int newLife) {
        life = newLife;
    }

    bool DecreaseLife(){
        this->life -= 1;
        if(this->life == 0){
           return true;
        }
        return false;
    }

    bool moveSquares(int heightSquare){
        for (int i=0; i<4; i++){
            this->segments[i]->position.y =  this->segments[i]->position.y - heightSquare - 3;
        }
        this->coordLife.y = this->coordLife.y - heightSquare - 3;

        if(this->coordLife.y < 50){//game over
            return true;
        }
        return false;
    }

    // Destrutor para liberar a memória alocada para os segmentos de reta
    ~Square() {
        for (int i = 0; i < 4; i++) {
            delete segments[i];
        }
    }
};


#endif // SQUARE_H_INCLUDED
