/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2024
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Projectile.h"
#include "Line.h"
#include "Cannon.h"
#include "Square.h"
#include "Frames.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 800;

int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.

Vector2 coordMouse, pointIniCannon(0,0) /*,normal*/;

Frames  * frames;
Cannon *cannon = NULL;
Square *square = NULL;

Projectile *projectile = NULL;
std::vector<Projectile*> bolls;
std::vector<Line*> lines; //pra bola não sair por cima e pelos lado da tela
std::vector<Square*> squares;

Vector2  point(500, 0), direction(0, 700), point1(0, 700),direction1(500, 0), point2(0, 0), direction2(0, 700);

Line *left = new Line(point, direction, 13);
Line *top = new Line(point1, direction1, 13);
Line *right = new Line(point2, direction2, 13);

int numSquares=8, distanceSquare=3, widthSquare=(screenWidth-numSquares*3)/numSquares, numShots=0, limitShots=1;
float s, t, fps, controlFPS=500.0;
bool b, deleteSquade = false, gameOver=false;
char str_fps[5]; //teste para mostrar os fps

//assinatura de funções
void calculateReflectionVector(int i, Line *line);
void createSquares();

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    CV::color(0);

    fps = frames->getFrames();
    snprintf(str_fps, sizeof(str_fps), "%f.2", fps);
    CV::text( 50, screenHeight - 50, "FPS: ");
    CV::text( 100, screenHeight - 50, str_fps);

    cannon->draw(coordMouse, pointIniCannon);

    for (int j=0; j<lines.size(); j++){
        lines[j]->draw();
    }

    for (int k=0; k < squares.size(); k++){
        squares[k]->draw();
    }
    for ( int i = 0; i < bolls.size(); i++){
        bolls[i]->Anima();
        for (int j = 0; j < lines.size(); j++){ //linhas de controle pra não sair da tela
            b = lines[j]->intersects(*bolls[i], s, t);
            if(b){
                calculateReflectionVector(i, lines[j]);
            }
        }
        for (auto it = squares.begin(); it != squares.end(); ) { //vetor quadrados
            Square* square = *it;
            bool deleteSquare = false;
            for (int j = 0; j < 4; j++) {
                bool intersects = square->segments[j]->intersects(*bolls[i], s, t);
                if (intersects) {
                    calculateReflectionVector(i, square->segments[j]);
                    deleteSquare = square->DecreaseLife();
                    if (deleteSquare) {
                        // Exclua o quadrado e avance o iterador
                        delete square;
                        it = squares.erase(it);
                        deleteSquade = true;
                        // Saia do loop interno, pois o quadrado foi excluído
                        break;
                    }
                }
            }
            // Avance para o próximo elemento se o quadrado não foi excluído
            if (!deleteSquare) {
                ++it;
            }
        }
        if (bolls[i]->position.y < 0){//excluir bolas que não estão mais na tela
            float aux = bolls[i]->position.x;
            delete bolls[i];
            bolls.erase(bolls.begin() + i);
            --i;
            if(numShots >= limitShots){ //cria um nova fila e quadrados e baix aos que já tem
                numShots = 0;
                limitShots += 1;
                pointIniCannon.set(aux, 0);
                //coloca nova fileira de quadrados
                for(int i=0; i<squares.size(); i++){
                    gameOver = squares[i]->moveSquares(widthSquare);
                }
                createSquares();
            }
        }
    }
    // Dorme por um tempo para alcançar o FPS desejado neste caso 60
    int sleepTime = (int)((1.0 / controlFPS - 1.0 / fps) * 1000);
    if (sleepTime > 0) {
        Sleep(sleepTime);
    }
}

void createSquares(){
    Vector2 topLeft(2, 695);

    for(int i=0; i<numSquares; i++){
        square = new Square(topLeft, widthSquare, widthSquare, 1);
        squares.push_back(square);
        topLeft.x = topLeft.x + widthSquare+distanceSquare;
    }
}

//calcula o vetor reflexão
void calculateReflectionVector(int i, Line *line){
    // Calcula o ponto de colisão da bola com a linha
    Vector2 collisionPoint = bolls[i]->getPosition();
    // Calcula o vetor direção da linha
    Vector2 lineDirection = line->getDirection();
    lineDirection.normalize();
    // Vetor da bola para o ponto de colisão
    Vector2 ballToCollision = collisionPoint - line->getPosition();
    // Calcula o componente da bola na direção perpendicular à linha
    Vector2 perpendicularComponent = ballToCollision - (lineDirection * ballToCollision.dot(lineDirection));
    // Calcula o ponto d//seta para baixoe reflexão na linha
    Vector2 reflectionPoint = collisionPoint - perpendicularComponent;
    // Vetor direção da bola para o ponto de reflexão
    Vector2 directionToReflection = reflectionPoint - collisionPoint;
    directionToReflection.normalize();
    // Calcula o vetor de reflexão usando a fórmula de reflexão
    Vector2 reflection = bolls[i]->getDirection() - (directionToReflection * (2 * bolls[i]->getDirection().dot(directionToReflection)));
    // Atualiza a direção da bola com o vetor de reflexão
    bolls[i]->setDirection(reflection);
}


//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key){
    switch(key){
        case 201: //seta para cima aumenta o limite de FPS
            controlFPS++;
            break;

        case 203: //seta para baixo diminui o limite de FPS
            controlFPS--;
            break;
    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   coordMouse.x = x;
   coordMouse.y = y;

   if(!button && !state){//cria projetil
        projectile =  new Projectile(cannon->getPoint1(), cannon->getPoint2());
        bolls.push_back(projectile);
        numShots++;
   }
}
int main(void)
{
    cannon = new Cannon();

    //pra não sair da tela
    lines.push_back(left);
    lines.push_back(top);
    lines.push_back(right);

    createSquares();

    frames  = new Frames();

   CV::init(screenWidth, screenHeight, "T3 Ivan Maidana da Silveira");
   CV::run();
}
