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
float moment;

PointLoad() = default;
PointLoad(Vector2 f, float m) : force(f), moment(m) {};



void Draw(Vector2 position, Camera2D camera) const override{
    drawFixedSizeAnnotadedArrow(position, force, loadScale, false, raioNo * 2, raioNo / 1, raioNo * 0.5f, forceColor, camera);
    drawFixedSizeAnnotadedMoment(position, moment, raioNo * 5, raioNo * 2, raioNo / 1, raioNo * 0.5f, momentColor, camera);
}




template<class Archive>
void serialize(Archive& ar) {
    ar(cereal::make_nvp("force", force),
       cereal::make_nvp("moment", moment));
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