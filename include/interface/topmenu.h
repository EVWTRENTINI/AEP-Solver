#pragma once
#include "utils.h"
#include "imgui.h"
#include "globals.h"
#include "section.h"

enum class ToolMode
{
	None,
	NodeAdd,
	NodeRem,
	NodeMov,
	BeamAdd,
	BeamRem,
	SupportNone,
	SupportLx,
	SupportLy,
	SupportLxy,
	SupportTv,
	SupportTh,
	PointLoadAdd,
	PointLoadRem,
	ShowNormal,
	ShowShear,
	ShowBending,
};

ToolMode toolMode = ToolMode::None;
bool ToolModeChanged = false;
bool isAnalysisUpToDate = false;

class TopMenuWindow : public DocumentWindow
{
public:
	float cursorPosX;	
	const char* titulo;
	float padding = 3.0f;
	int Nbuttons;
	int buttonWidth;
	ToolMode lastToolMode = ToolMode::None;
	Vector2 force = {0, 0};
	bool isBeingInteracted;
	MaterialManager* materialManager;
	SectionManager* sectionManager;
	int materials_current_idx = 0;
	int sections_current_idx = 0;

	

	TopMenuWindow(MaterialManager* matMan, SectionManager* secMan) : materialManager(matMan), sectionManager(secMan) {}

	void VerticalSeparator() {
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 currentSpacing = style.ItemSpacing;
			ImGui::SameLine(); 
			float line_height = ImGui::GetItemRectSize().y;
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddLine(ImVec2(p.x -currentSpacing.x/2 -1, p.y), ImVec2(p.x-currentSpacing.x/2 -1, p.y + line_height), IM_COL32(150, 150, 150, 255));
			ImGui::SameLine();
		}
	
	void Setup() override
	{
		
	}

	void Shutdown() override
	{

	}

	void Show() override
	{	
		isBeingInteracted = false;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));
		ImGui::SetNextWindowPos(ImVec2(0, mainMenuSize)); 
        ImGui::SetNextWindowSize(ImVec2(GetScreenWidth(), topMenuSize));
		
		
		    if (ImGui::Begin("Top Menu", &Open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
    		{
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.f)); // Borda transparente
				ImVec2 currentSpacing = ImVec2(5, 5);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, currentSpacing);
    		    if (ImGui::BeginTabBar("TopMenuTabBar"))
    		    {		
    		        if (ImGui::BeginTabItem("Editar"))
    		        {
						titulo = "Nós";
						Nbuttons = 3;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
    		            	if (ImGui::Button("Adicionar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::NodeAdd;}
							ImGui::SameLine();
    		            	if (ImGui::Button("Remover", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::NodeRem;}
    		            	ImGui::SameLine();
							if (ImGui::Button("Mover", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::NodeMov;}
							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild();

    		            VerticalSeparator();

    		            titulo = "Barras";
						Nbuttons = 2;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
    		            	if (ImGui::Button("Adicionar", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::BeamAdd;}
							ImGui::SameLine();
    		            	if (ImGui::Button("Remover", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::BeamRem;}
							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild(); 

    		            VerticalSeparator();

						titulo = "Apoios";
						Nbuttons = 6;
						buttonWidth = 90;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
							if (ImGui::Button("Nenhum", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::SupportNone;}
							ImGui::SameLine();
    		            	if (ImGui::Button("Translação x", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::SupportLx;}
							ImGui::SameLine();
    		            	if (ImGui::Button("Translação y", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::SupportLy;}
							ImGui::SameLine();
    		            	if (ImGui::Button("Translação x-y", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::SupportLxy;}
							ImGui::SameLine();
    		            	if (ImGui::Button("Engaste ver", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::SupportTv;}
							ImGui::SameLine();
    		            	if (ImGui::Button("Engaste hor", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::SupportTh;}

							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild(); 

						VerticalSeparator();

    		            ImGui::EndTabItem();
    		        }
					if (ImGui::BeginTabItem("Carregamento"))
    		        {
    		            titulo = "Concentrado em Nó";
						const char* caption1 = "Força em x:";
						const char* caption2 = "Força em y:";
						int captionSize1 = ImGui::CalcTextSize(caption1).x;
						int captionSize2 = ImGui::CalcTextSize(caption2).x;
						int minCaptionSize = captionSize1;
						if (captionSize1<=captionSize2)
							minCaptionSize = captionSize2;
						Nbuttons = 3;
						buttonWidth = 90;
    		            ImGui::BeginChild(titulo, ImVec2((padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x + minCaptionSize + currentSpacing.x), 0), true, ImGuiWindowFlags_NoScrollbar);
    						ImGui::PushItemWidth(buttonWidth);  // Define a largura do próximo item para buttonWidth
							// Campo de texto para Força em x
							ImGui::BeginGroup();
							ImGui::Text(caption1); ImGui::SameLine();
    						if (ImGui::InputFloat("##xForce", &force.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal)) {
    						    
    						}

    						// Campo de texto para Força em y
							ImGui::Text(caption2); ImGui::SameLine();
    						if (ImGui::InputFloat("##yForce", &force.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal)) {
    						    
    						}

    						ImGui::PopItemWidth();  // Restaura a largura do item para o padrão
							ImGui::EndGroup();
							ImGui::SameLine();
							if (ImGui::Button("Adicionar", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::PointLoadAdd;}

							ImGui::SameLine();
    		            	if (ImGui::Button("Remover", ImVec2(buttonWidth,  ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::PointLoadRem;
							}
							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild(); 

    		            VerticalSeparator();

						titulo = "Escala";
						Nbuttons = 1;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
    		            	ImGui::VSliderFloat("##v", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y), &loadScale, 0.0f, 100.0f, "%.1f\npixels");
							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild();

    		            VerticalSeparator();						

    		            ImGui::EndTabItem();
    		        }

					if (ImGui::BeginTabItem("Material e Seção Transversal"))
    		        {
    		            titulo = "Material";
						Nbuttons = 5;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
							ImGui::BeginGroup();
							
							std::vector<const char*> materialNames;
                    		for (const auto& material : materialManager->materials) {
                    		    materialNames.push_back(material->name.c_str());
                    		}

                    		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
                    		const char* materials_preview_value = materialNames[materials_current_idx];
                    		ImGui::SetNextItemWidth(3 * buttonWidth + 2 * currentSpacing.x);
                    		if (ImGui::BeginCombo("##Material selector", materials_preview_value)) {
                    		    isBeingInteracted = true;
                    		    for (size_t n = 0; n < materialNames.size(); n++) {
                    		        const bool is_selected = (static_cast<size_t>(materials_current_idx) == n);
                    		        if (ImGui::Selectable(materialNames[n], is_selected))
                    		            materials_current_idx = static_cast<int>(n);

                    		        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    		        if (is_selected)
                    		            ImGui::SetItemDefaultFocus();
                    		    }
                    		    ImGui::EndCombo();
                    		}

							ImGui::BeginGroup();
    		            	if (ImGui::Button("Editar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
    		            	ImGui::SameLine();
							if (ImGui::Button("Duplicar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							ImGui::SameLine();
							if (ImGui::Button("Remover", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							ImGui::EndGroup();
							ImGui::EndGroup();
							ImGui::SameLine();
							if (ImGui::Button("Criar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							ImGui::SameLine();
							if (ImGui::Button("Atribuir", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							

							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild();

    		            VerticalSeparator();

						titulo = "Seção Transversal";
						Nbuttons = 5;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
    		            	
							ImGui::BeginGroup();
							std::vector<const char*> sectionNames;
                    		for (const auto& section : sectionManager->sections) {
                    		    sectionNames.push_back(section->name.c_str());
                    		}

                    		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
                    		const char* sections_preview_value = sectionNames[sections_current_idx];
                    		ImGui::SetNextItemWidth(3 * buttonWidth + 2 * currentSpacing.x);
                    		if (ImGui::BeginCombo("##Material selector", sections_preview_value)) {
                    		    isBeingInteracted = true;
                    		    for (size_t n = 0; n < sectionNames.size(); n++) {
                    		        const bool is_selected = (static_cast<size_t>(sections_current_idx) == n);
                    		        if (ImGui::Selectable(sectionNames[n], is_selected))
                    		            sections_current_idx = static_cast<int>(n);

                    		        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    		        if (is_selected)
                    		            ImGui::SetItemDefaultFocus();
                    		    }
                    		    ImGui::EndCombo();
                    		}
							ImGui::BeginGroup();
    		            	if (ImGui::Button("Editar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
    		            	ImGui::SameLine();
							if (ImGui::Button("Duplicar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							ImGui::SameLine();
							if (ImGui::Button("Remover", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							ImGui::EndGroup();
							ImGui::EndGroup();
							ImGui::SameLine();
							if (ImGui::Button("Criar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							ImGui::SameLine();
							if (ImGui::Button("Atribuir", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								
							}
							

							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
							
    		            ImGui::EndChild();
						VerticalSeparator();
    		            ImGui::EndTabItem();
    		        }

    		        if (ImGui::BeginTabItem("Análise"))
    		        {
						titulo = "Esforço Normal";
						Nbuttons = 2;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
    		            	if (ImGui::Button("Visualizar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::ShowNormal;
							}
							ImGui::SameLine();
							ImGui::VSliderFloat("##normalScale", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y), &normalScale, 0.0f, 100.0f, "%.1f\npixels");
							
							
							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild();

    		            VerticalSeparator();

						titulo = "Esforço Cortante";
						Nbuttons = 2;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
    		            	if (ImGui::Button("Visualizar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::ShowShear;
							}
							ImGui::SameLine();
							ImGui::VSliderFloat("##shearScale", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y), &shearScale, 0.0f, 100.0f, "%.1f\npixels");
							
							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild();

    		            VerticalSeparator();

						titulo = "Momento Fletor";
						Nbuttons = 2;
						buttonWidth = 70;
    		            ImGui::BeginChild(titulo, ImVec2(padding * 2 + Nbuttons * buttonWidth + (Nbuttons - 1) * currentSpacing.x, 0), true, ImGuiWindowFlags_NoScrollbar);
    		            	if (ImGui::Button("Visualizar", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y))){
								toolMode = ToolMode::ShowBending;
							}
							ImGui::SameLine();
							ImGui::VSliderFloat("##bendingScale", ImVec2(buttonWidth, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(titulo).y - currentSpacing.y), &bendingScale, 0.0f, 100.0f, "%.1f\npixels");
							
							cursorPosX = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(titulo).x) / 2;
                			ImGui::SetCursorPosX(cursorPosX);
                			ImGui::Text(titulo);
    		            ImGui::EndChild();

    		            VerticalSeparator();


    		            ImGui::EndTabItem();
    		        }

					ImGui::PopStyleVar(2);
					ImGui::PopStyleColor(1);
    		        ImGui::EndTabBar();
					
    		    }
    		}
		ImGui::End();
		
		if (!ToolModeChanged){
			ToolModeChanged = !(lastToolMode == toolMode);
		}else{
			isAnalysisUpToDate = false;
		}
	}

	void Update() override
	{
		if (!Open)
			return;

		lastToolMode = toolMode;

		if (IsKeyDown(KEY_ESCAPE) && (toolMode != ToolMode::ShowNormal && toolMode != ToolMode::ShowShear && toolMode != ToolMode::ShowBending))
			toolMode = ToolMode::None;

			
		//std::cout << static_cast<int>(CurrentToolMode) << std::endl;
	}
 
	Texture2D GridTexture = { 0 };
};
