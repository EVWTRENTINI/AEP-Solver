#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include "raylib.h"
#include "utils.h"
#include "node.h"

class NodeGuide{
public:
	Vector2 position;
	double creationTime;
	bool activeX;
	bool activeY;

    NodeGuide(Vector2 pos, double time) : position(pos), creationTime(time), activeX(false), activeY(false) {}


};

class NodeGuideManager{
public:
int lastNodeSelected = -1;
double hoverStartTime = 0.0;
bool alreadyCreated = false;
float timeToCreate = 1;
bool drawNodeGuideSelector = false;
bool drawDistanceSelector = false;
SelectorToDraw selectorToDraw;
NodeGuide* closestGuideX = nullptr;
NodeGuide* closestGuideY = nullptr;
std::vector<SelectorToDraw> selectors;


std::vector<NodeGuide> guides;

void AddGuideLine(Vector2 position)
{
	std::vector<NodeGuide>::iterator it = std::find_if(guides.begin(), guides.end(), [&position](const NodeGuide &guide)
													   { return guide.position.x == position.x && guide.position.y == position.y; });
	if (it != guides.end()){
		guides.erase(it);
	} else {
			guides.emplace_back(position, GetTime());
	}
}

void UpdateGuides(const std::vector<std::shared_ptr<Node>> *nodes, Vector2 mousePosition, Camera2D camera)
{
	if (IsKeyPressed(KEY_ESCAPE))	{
		guides.clear();
	}

	for (NodeGuide &guide : guides) {

        // Verifica a distancia vertical para a linha horizontal
        if (std::abs(mousePosition.y - guide.position.y) <= 10 / camera.zoom) {
            guide.activeY = true;
        } else {
            guide.activeY = false;
        }

        // Verifica a distancia horizontal para a linha vertical
        if (std::abs(mousePosition.x - guide.position.x) <= 10 / camera.zoom) {
            guide.activeX = true;
        } else {
            guide.activeX = false;
        }
    }
	drawNodeGuideSelector = false;
	if (nodes)	{
		for (const std::shared_ptr<Node>& node : *nodes)
		{
			if (node->IsNearby(mousePosition, raioNo, camera.zoom))	{
				if (lastNodeSelected != node->id){
					lastNodeSelected = node->id;
					hoverStartTime = GetTime();
				}
				else if ((GetTime() - hoverStartTime) >= timeToCreate){
					if (!alreadyCreated) {
					alreadyCreated = true;
					AddGuideLine(node->position);
					}
				}else{
					float alphaValue = ((GetTime() - hoverStartTime) / timeToCreate*2) * 255.0f;
					int alphaInt = static_cast<int>(round(alphaValue));
					alphaInt = Clamp(alphaInt, 0, 255);
					drawNodeGuideSelector = true;
					selectorToDraw.center = node->position;
					selectorToDraw.camera = camera;
					selectorToDraw.color = Color{255, 255, 0, static_cast<unsigned char>(alphaInt)};
				}
			}
			else
			{
				if (lastNodeSelected == node->id){
					lastNodeSelected = -1;
					hoverStartTime = 0.0; // Reseta o temporizador
					alreadyCreated = false;
				}
			}
		}
	}


}

void FindClosestGuides(Vector2 mousePosition, Camera2D camera){
	float minDistX = std::numeric_limits<float>::max();
	float minDistY = std::numeric_limits<float>::max();

	closestGuideX = nullptr;
	closestGuideY = nullptr;

	for (auto& guide : guides) {
        if (guide.activeX) {
            float distX = std::abs(guide.position.x - mousePosition.x);
            if (distX < minDistX) {
                minDistX = distX;
                closestGuideX = &guide;
            }
        }

        if (guide.activeY) {
            float distY = std::abs(guide.position.y - mousePosition.y);
            if (distY < minDistY) {
                minDistY = distY;
                closestGuideY = &guide;
            }
        }
    }

	if (closestGuideX){
		drawDistanceSelector = true;
		selectors.emplace_back(closestGuideX->position, camera, Color{255, 255, 0, 255});
	}
	if (closestGuideY){
		drawDistanceSelector = true;
		selectors.emplace_back(closestGuideY->position, camera, Color{255, 255, 0, 255});
	}
	if (closestGuideX && closestGuideY){
		drawDistanceSelector = false;
		selectors.clear();
	}
	

}

void DrawGuides(Camera2D camera)
{
	BeginMode2D(camera);
	double currentTime = GetTime();
	for (const NodeGuide &guide : guides)
	{
		double elapsedTime = currentTime - guide.creationTime;
		float alpha = elapsedTime / 0.2f;
		alpha = Clamp(alpha, 0.0f, 1.0f);
		alpha = EaseInOutQuad(alpha);
        unsigned char alphaIntX = static_cast<unsigned char>((guide.activeX ? 1.0f : 0.2f) * alpha * 255);
        unsigned char alphaIntY = static_cast<unsigned char>((guide.activeY ? 1.0f : 0.2f) * alpha * 255);

        Color guideColorX = {255, 127, 39, alphaIntX};
        Color guideColorY = {255, 127, 39, alphaIntY};

		float thickness = 10.0f + (2.0f - 10.0f) * alpha;

		Vector2 worldTopLeftMost = GetScreenToWorld2D(Vector2{0, 0}, camera);
		Vector2 worldBottomRightMost = GetScreenToWorld2D(Vector2{static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, camera);

        float left = guide.position.x + alpha * (worldTopLeftMost.x - guide.position.x);
        float right = guide.position.x + alpha * (worldBottomRightMost.x - guide.position.x);
        float top = guide.position.y + alpha * (worldTopLeftMost.y - guide.position.y);
        float bottom = guide.position.y + alpha * (worldBottomRightMost.y - guide.position.y);

        DrawLineEx(Vector2{left, guide.position.y}, Vector2{right, guide.position.y}, thickness / camera.zoom, guideColorY);
        DrawLineEx(Vector2{guide.position.x, top}, Vector2{guide.position.x, bottom}, thickness / camera.zoom, guideColorX);
	}
	EndMode2D();
}
};