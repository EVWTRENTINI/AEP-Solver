#pragma once
#include <raylib.h>
#include "globals.h"
#include "utils.h"

class Node;
class Beam;

class NodeLoad
{
public:
NodeLoad() = default;

virtual void Draw(Vector2 position, Camera2D camera) const = 0;

};

/*class BeamLoad
{
public:
BeamLoad() = default;

virtual void Draw(const Beam& beam, Camera2D camera) const = 0;

};*/

class PointLoad : public NodeLoad {
public:

Vector2 force;

PointLoad() = default;
PointLoad(Vector2 f) : force(f) {};

void Draw(Vector2 position, Camera2D camera) const override{
    if (force.x != 0 || force.y != 0){
        // Calcula a posição final baseada no deslocamento e na escala
        Vector2 endPosition = {position.x - force.x * loadScale / camera.zoom, position.y + force.y * loadScale / camera.zoom};

        // Desenha a linha do ponto final de volta ao ponto inicial
        

        // Ângulo de rotação da seta em relação ao eixo horizontal
        float angle = atan2(-force.y, force.x);

        // Comprimento e largura do triângulo da ponta da seta
        float arrowLength = raioNo * 2 / camera.zoom;  // Comprimento do triângulo
        float arrowWidth = raioNo / 1 / camera.zoom;   // Metade da largura do triângulo

        // Pontos do triângulo no início da linha
        Vector2 p1 = {position.x, position.y};
        Vector2 p2 = {position.x - arrowLength, position.y - arrowWidth};
        Vector2 p3 = {position.x - arrowLength, position.y + arrowWidth};

        // Rotacionar os pontos p2 e p3 em torno de p1 pelo ângulo calculado
        p2 = RotatePoint(p1, p2, angle);
        p3 = RotatePoint(p1, p3, angle);
        BeginMode2D(camera);  
        DrawLineEx(endPosition, position, raioNo * 0.5f / camera.zoom, RED);
        DrawTriangle(p1, p2, p3, RED);
        DrawTriangle(p3, p2, p1, RED);
        EndMode2D();
    }
}

template<class Archive>
void serialize(Archive& ar) {
    ar(cereal::make_nvp("force", force));
};

};

/*class DistributedLoad : public BeamLoad {
public:
float xForce, yForce;

DistributedLoad() = default;
DistributedLoad(float xF, float yF) : xForce(xF), yForce(yF) {};

};*/


/*

class LoadManager
{
public:
std::vector<std::shared_ptr<Load>> loads;

LoadManager() = default;



};*/