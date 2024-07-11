#pragma once
#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include <iostream>


class DistanceInputWindow{
public:
bool isDistanceWindowOn = false;
char distanceInput[30] = "";
Vector2 screenPosition;
Vector2 clickWorldPosition;
Vector2 nodeWorldPosition;
bool isX;
bool isFirstTimeOpened = true;
float distance;
bool createNode = false;


void draw(){
	if (isDistanceWindowOn){
            ImGui::SetNextWindowPos(ImVec2(screenPosition.x - 175/2, screenPosition.y - 65));
            ImGui::Begin("Criar nó", &isDistanceWindowOn, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::Text("Distância do nó selecionado:");
			if (isFirstTimeOpened) {
    			ImGui::SetKeyboardFocusHere();
    			isFirstTimeOpened = false;
				if (isX) {
                	snprintf(distanceInput, sizeof(clickWorldPosition), "%.3f", std::abs(clickWorldPosition.y - nodeWorldPosition.y));
           		} else {
                snprintf(distanceInput, sizeof(distanceInput), "%.3f", std::abs(clickWorldPosition.x - nodeWorldPosition.x));
            	}
			}
			ImGui::PushItemWidth(175);
            ImGui::InputText("##distance", distanceInput, IM_ARRAYSIZE(distanceInput));
			ImGui::PopItemWidth(); // Restaura a largura padrão para os próximos itens
			float buttonWidth = ImGui::GetContentRegionAvail().x / 2 - ImGui::GetStyle().ItemSpacing.x / 2;
            if (ImGui::Button("Confirmar", ImVec2(buttonWidth, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
				std::cout << "Botão Confirmar pressionado!" << std::endl;
            	distance = atof(distanceInput);
				if (isX) {
					if (nodeWorldPosition.y - clickWorldPosition.y > 0){
						distance = -distance;
					}
				}else{
					if (nodeWorldPosition.x - clickWorldPosition.x > 0){
						distance = -distance;
					}
				}

				createNode = true;
                isDistanceWindowOn = false;
				isFirstTimeOpened = true;
            }
			ImGui::SameLine();
            if (ImGui::Button("Cancelar", ImVec2(buttonWidth, 0)) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                isDistanceWindowOn = false;
				isFirstTimeOpened = true;
            }
            ImGui::End();
	}else {
        isFirstTimeOpened = true;
    }
	
}
};