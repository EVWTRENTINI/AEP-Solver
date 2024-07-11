#pragma once
#include "raylib.h"
#include "raymath.h"
#include <cereal/cereal.hpp>
#include "load.h"

enum class Support
	{
        None,
		Lx,
		Ly,
		Lxy,
		Tv,
		Th,
	};


class Node {
public:
static int count;
int id;
Vector2 position;
Support support;
PointLoad load;


Node() : id(count++), position({0, 0}), support(Support::None), load(PointLoad(Vector2{0, 0})) {}

Node(Vector2 pos) : id(count++), position(pos), support(Support::None), load(PointLoad(Vector2{0, 0})) {
    std::cout << "Criando Node com número: " << id << ", x: " << position.x << ", y: " << position.y << std::endl;
}

void Draw(Camera2D camera) const {
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;

	BeginMode2D(camera);
    switch (support) {
        case Support::None:
            DrawCircleV(position, raioNo/camera.zoom, green);
		    break;

        case Support::Lx:
            
			v1 = {position.x, position.y};
            v2 = {position.x-15/camera.zoom, position.y-raioNo*2/camera.zoom};
            v3 = {position.x-15/camera.zoom, position.y+raioNo*2/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen); 
            DrawCircleV(position, raioNo/camera.zoom/2, green);
            break;

        case Support::Ly:
            
			v1 = {position.x, position.y};
            v2 = {position.x-raioNo*2/camera.zoom, position.y+raioNo*3/camera.zoom};
            v3 = {position.x+raioNo*2/camera.zoom, position.y+raioNo*3/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen); 
            DrawCircleV(position, raioNo/camera.zoom/2, green);
			break;

        case Support::Lxy:
            
			v1 = {position.x, position.y};
            v2 = {position.x-raioNo*3/camera.zoom, position.y-raioNo*2/camera.zoom};
            v3 = {position.x-raioNo*3/camera.zoom, position.y+raioNo*2/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen); 
            v1 = {position.x, position.y};
            v2 = {position.x-raioNo*2/camera.zoom, position.y+raioNo*3/camera.zoom};
            v3 = {position.x+raioNo*2/camera.zoom, position.y+raioNo*3/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen); 
            DrawCircleV(position, raioNo/camera.zoom/2, green);
			break;

        case Support::Tv:
			v1 = {position.x + raioNo*.5f/camera.zoom, position.y - raioNo*3/camera.zoom};
            v2 = {position.x - raioNo*.5f/camera.zoom, position.y - raioNo*3/camera.zoom};
            v3 = {position.x + raioNo*.5f/camera.zoom, position.y + raioNo*3/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen);
			v1 = {position.x - raioNo*.5f/camera.zoom, position.y - raioNo*3/camera.zoom};
            v2 = {position.x - raioNo*.5f/camera.zoom, position.y + raioNo*3/camera.zoom};
            v3 = {position.x + raioNo*.5f/camera.zoom, position.y + raioNo*3/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen); 
			break;

        case Support::Th:
			v1 = {position.x + raioNo*3/camera.zoom, position.y - raioNo*.5f/camera.zoom};
            v2 = {position.x - raioNo*3/camera.zoom, position.y - raioNo*.5f/camera.zoom};
            v3 = {position.x + raioNo*3/camera.zoom, position.y + raioNo*.5f/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen);
			v1 = {position.x - raioNo*3/camera.zoom, position.y - raioNo*.5f/camera.zoom};
            v2 = {position.x - raioNo*3/camera.zoom, position.y + raioNo*.5f/camera.zoom};
            v3 = {position.x + raioNo*3/camera.zoom, position.y + raioNo*.5f/camera.zoom};
            DrawTriangle(v1, v2, v3, darkGreen); 
			break;

	}
	EndMode2D();
}

bool IsNearby(Vector2 target, float radius, float zoom) const {
    return Vector2Distance(position, target) <= radius/zoom;
}

void AddLoad(Vector2 f){
    load.force = f;
}

void DrawLoad(Camera2D camera) const{
    load.Draw(position, camera);
}
    
template<class Archive>
void serialize(Archive& ar) {
    ar(cereal::make_nvp("id", id),
       cereal::make_nvp("position", position),
       cereal::make_nvp("support", support),
       cereal::make_nvp("load", load)
       );
}
};


class NodeManager {
public:
    std::vector<std::shared_ptr<Node>> nodes;
	bool drawNodeSelector = false;

    bool AddNode(Vector2 position) {
        auto it = std::find_if(nodes.begin(), nodes.end(), [&](const std::shared_ptr<Node>& node) {
            return node->position.x == position.x && node->position.y == position.y;
        });

        // Só adicione o nó se não encontrar um nó na mesma posição
        if (it == nodes.end()) {
            nodes.emplace_back(std::make_shared<Node>(position));
			return true;
		}else{
			return false;
        }
    }

    void RemoveNode(const std::vector<std::shared_ptr<Node>>& nodesToRemove) {
        for (auto& node : nodesToRemove) {
            nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
        }
    }

    void DrawNodes(Camera2D camera) const {
        for (const auto& node : nodes) {
            node->Draw(camera);
        }
    }

	void DrawLoads(Camera2D camera) const{
		for (const auto& node : nodes) {
			node->DrawLoad(camera);
		}
	}


	std::shared_ptr<Node> FindClosestNode(Vector2 mouseWorldPosition, Camera2D camera){
		float minDist = std::numeric_limits<float>::max();
			
		std::shared_ptr<Node> closestNode = nullptr;

		for (auto& node : nodes) {
	        if (node->IsNearby(mouseWorldPosition, raioNo, camera.zoom)) {
	            float dist = Vector2DistanceSqr(node->position, mouseWorldPosition);
	            if (dist < minDist) {
	                minDist = dist;
	                closestNode = node;
	            }
	        }
	    }

		return closestNode;
	}
	template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("nodes", nodes));
    }
};

