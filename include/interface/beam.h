#pragma once
#include <algorithm>
#include <limits>
#include "eigenpch.h"

#include "interface/globals.h"
#include "interface/node.h"
#include "interface/load.h"
#include "interface/material.h"
#include "interface/section.h"

class Beam{
public:
std::shared_ptr<Node> startNode;
std::shared_ptr<Node> endNode;
std::shared_ptr<BeamMaterial> material;
std::shared_ptr<BeamSection> section;
Eigen::Matrix2d r;
float length;



Beam() = default;

Beam(std::shared_ptr<Node> start, std::shared_ptr<Node> end, std::shared_ptr<BeamMaterial> mat, std::shared_ptr<BeamSection> sec) : startNode(start), endNode(end), material(mat), section(sec) {
    UpdateBeam();
}

void UpdateBeam(){
    Eigen::Vector2d direction(endNode->position.x - startNode->position.x, endNode->position.y - startNode->position.y);
    length = direction.norm();
    direction.normalize();
    r << direction.x(), direction.y(),
        -direction.y(), direction.x();
}

void displayInfo() {
        std::cout << "A barra começa no nó: " << startNode->id << ", termina no nó: " << endNode->id  << " e é do material:"<< material->name << " Tem a seção:"<< section->name << std::endl;
}

void DrawFill(Camera2D camera) const{
    BeginMode2D(camera);
        DrawLineEx(startNode->position, endNode->position, raioNo/camera.zoom, green);
	EndMode2D();
}

void DrawOutline(Camera2D camera) const{
    BeginMode2D(camera);
        DrawLineEx(startNode->position, endNode->position, raioNo*1.4f/camera.zoom, darkGreen);
	EndMode2D();
}

template <class Archive>
    void serialize(Archive& ar) {
        ar(
            cereal::make_nvp("startNode", startNode),
            cereal::make_nvp("endNode", endNode),
            cereal::make_nvp("material", material),
            cereal::make_nvp("section", section)
        );
    }
};

class BeamManager{
public:
std::vector<Beam> beams;

void AddBeam(std::shared_ptr<Node> start, std::shared_ptr<Node> end, std::shared_ptr<BeamMaterial> mat, std::shared_ptr<BeamSection> sec) {
    auto it = std::find_if(beams.begin(), beams.end(), [start, end](const Beam& beam) {
        return (beam.startNode == start && beam.endNode == end) ||
               (beam.startNode == end && beam.endNode == start);
    });

    if (it == beams.end()) {
        beams.emplace_back(start, end, mat, sec);
    }else{
        std::cout << "Já existe uma barra igual." << std::endl;
    }
}

void Draw(Camera2D camera) const{
    for (const Beam& beam : beams) {
        beam.DrawOutline(camera);
    }
    for (const Beam& beam : beams) {
        beam.DrawFill(camera);
    }
}

void RemoveBeamsConnectedToNodes(const std::vector<std::shared_ptr<Node>>& nodesToRemove) {
    beams.erase(std::remove_if(beams.begin(), beams.end(), [&nodesToRemove](const Beam& beam) {
        return std::any_of(nodesToRemove.begin(), nodesToRemove.end(), [&](const std::shared_ptr<Node>& node) {
            return beam.startNode == node || beam.endNode == node;
        });
    }), beams.end());
}

float FindLowestYCoordinate() const{
    float lowestY = std::numeric_limits<float>::min(); // Inicializa com o maior valor possível para float
    for (const auto& beam : beams) {
        // Verifica o nó de início da barra
        if (beam.startNode->position.y > lowestY) {
            lowestY = beam.startNode->position.y;
        }
        // Verifica o nó de fim da barra
        if (beam.endNode->position.y > lowestY) {
            lowestY = beam.endNode->position.y;
        }
    }
    return lowestY; // Retorna a menor coordenada y encontrada
}

void DrawShadows(Camera2D camera) const{
    float yGround = FindLowestYCoordinate();
    for (const Beam& beam : beams) {
        BeginMode2D(camera);
        DrawLineEx(Vector2{beam.startNode->position.x + (beam.startNode->position.y - yGround)*.3f,
                           beam.startNode->position.y - (beam.startNode->position.y - yGround)*0.8f},
                   Vector2{beam.endNode->position.x + (beam.endNode->position.y - yGround)*.3f,
                           beam.endNode->position.y - (beam.endNode->position.y - yGround)*0.8f},
                        raioNo*1.4f/camera.zoom, Color{ 70, 70, 70, 255 });
	    EndMode2D();
    }
}

void UpdateBeams(){
    for (Beam& beam : beams) {
        beam.UpdateBeam();
    }
}

template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("beams", beams));
    }
};