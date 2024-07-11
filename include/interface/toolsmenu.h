#pragma once
#include "utils.h"
#include "imgui.h"
#include "globals.h"

bool isGridOn = true;
bool isGuideOn = true;

class ToolsMenuWindow : public DocumentWindow
{
public:
	
	void Setup() override
	{
		
	}

	void Shutdown() override
	{

	}

	void Show() override
	{
		float windowHeight = GetScreenHeight() - topPadding;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::SetNextWindowPos(ImVec2(0, topPadding)); 
        ImGui::SetNextWindowSize(ImVec2(leftMenuSize, windowHeight));
		
		if (ImGui::Begin("Menu Esquerdo", &Open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{

			Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
			// Desenha oq quiser
			if (windowHeight > 1){
				if (ImGui::BeginChild("Auxílios", ImVec2(ImGui::GetContentRegionAvail().x, windowHeight), ImGuiWindowFlags_NoScrollbar))
				{
					ImGui::SetCursorPosX(2);
					ImGui::SetCursorPosY(3);

					if (ImGui::Button("Gride", ImVec2(-1, 0)))	{
						isGridOn = !isGridOn;
					}

					if (ImGui::Button("Guias", ImVec2(-1, 0)))	{
						isGuideOn = !isGuideOn;
					}


					ImGui::EndChild();
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Update() override
	{
		if (!Open)
			return;
	}
 
	Texture2D GridTexture = { 0 };
};
