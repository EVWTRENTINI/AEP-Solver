#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <limits>

// Raylib
#include "raylib.h"
#include "raymath.h"

// ImGui
#include "imgui.h"
#include "rlImGui.h"
#include "imgui_impl_raylib.h"
#include "rlImGuiColors.h"

// Cereal
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>


// My includes
#include "interface/globals.h"
#include "interface/utils.h"
#include "interface/cameracontroller.h"
#include "interface/beam.h"
#include "interface/node.h"
#include "interface/load.h"
#include "interface/nodeguide.h"
#include "interface/backgroundgrid.h"
#include "interface/topmenu.h"
#include "interface/toolsmenu.h"
#include "interface/distanceinput.h"
#include "interface/material.h"
#include "interface/section.h"
#include "structuralanalysis.h"




template <class Archive>
void serialize(Archive & archive, Vector2 & v) {
    archive(
		cereal::make_nvp("x", v.x),
		cereal::make_nvp("y", v.y));
}
template <class Archive>
void serialize(Archive & archive, Camera2D & camera) {
    archive(
        cereal::make_nvp("offset", camera.offset),
        cereal::make_nvp("target", camera.target),
        cereal::make_nvp("rotation", camera.rotation),
        cereal::make_nvp("zoom", camera.zoom));
}


int Node::count = 0;


bool Quit = false;

bool ImGuiDemoOpen = false;



MaterialManager materialManager;
SectionManager sectionManager;

TopMenuWindow topMenu(&materialManager, &sectionManager);

ToolsMenuWindow toolsMenu;


class EditorWindow : public DocumentWindow
{
public:

float baseSpacing = 100.0f;
NodeGuideManager nodeGuideManager;
CameraController cameraController;
NodeManager nodeManager;
Vector2 worldPosition;
Vector2 mousePosition;
std::string positionText;
DistanceInputWindow distanceInputWindow;
std::shared_ptr<Node> closestNode;
Vector2 worldGridPosition;
BeamManager beamManager;
std::shared_ptr<Node> beamStart = nullptr;
std::shared_ptr<Node> beamEnd = nullptr;
bool drawCursorOnClosestNode = false;
std::vector<std::shared_ptr<Node>> nodesToRemove;
bool connectBeamToLastNode;
TopMenuWindow* topMenu;
MaterialManager* materialManager;
SectionManager* sectionManager;
StructuralAnalysis analysis;



	EditorWindow(TopMenuWindow* menu, MaterialManager* matMan, SectionManager* secMan) : topMenu(menu), materialManager(matMan), sectionManager(secMan) {}

	void clear(ToolMode tool){
		beamStart = nullptr;
		beamEnd = nullptr;
		ToolModeChanged = false;
		isAnalysisUpToDate = false;
		toolMode = tool;
	}

	bool AddNode(){

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			nodeGuideManager.FindClosestGuides(GetScreenToWorld2D(mousePosition, cameraController.camera), cameraController.camera);
			if (nodeGuideManager.closestGuideX && nodeGuideManager.closestGuideY)	{
				return nodeManager.AddNode(Vector2{nodeGuideManager.closestGuideX->position.x, nodeGuideManager.closestGuideY->position.y});
			}
			else if (nodeGuideManager.closestGuideX || nodeGuideManager.closestGuideY){
				distanceInputWindow.isDistanceWindowOn = true;
				distanceInputWindow.screenPosition = mousePosition;
				distanceInputWindow.clickWorldPosition = GetScreenToWorld2D(mousePosition, cameraController.camera);
				if (nodeGuideManager.closestGuideX)	{
					distanceInputWindow.isX = true;
					distanceInputWindow.nodeWorldPosition = nodeGuideManager.closestGuideX->position;
				} else {
					distanceInputWindow.isX = false;
					distanceInputWindow.nodeWorldPosition = nodeGuideManager.closestGuideY->position;
				}
			} else {
				if (isGridOn) {
					return nodeManager.AddNode(worldGridPosition);
				} else {
					return nodeManager.AddNode(worldPosition);
				}
			}
		}
		return false;
	}


	void RemNode(){
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			for (auto &node : nodeManager.nodes){
				if (node->IsNearby(GetScreenToWorld2D(mousePosition, cameraController.camera), raioNo, cameraController.camera.zoom)){
					nodesToRemove.push_back(node);
				}
			}
		}
		beamManager.RemoveBeamsConnectedToNodes(nodesToRemove);
		nodeManager.RemoveNode(nodesToRemove);
		
		nodesToRemove.clear();
	}

	void concludeNodeCreation(){
		std::cout << "Nó final anotado e criando barra" << std::endl;
		beamManager.AddBeam(beamStart, beamEnd, materialManager->materials[topMenu->materials_current_idx], sectionManager->sections[topMenu->sections_current_idx]);
    	beamStart = beamEnd;
    	beamEnd = nullptr;
	}
		
	void AddBeam(){
		closestNode = nodeManager.FindClosestNode(worldPosition, cameraController.camera);
		drawCursorOnClosestNode = true;		
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))	{	
			if (beamStart == nullptr) {
    		    if (closestNode != nullptr) {
    		        beamStart = closestNode;
    		        std::cout << "Nó inicial anotado" << std::endl;
				}else{
					if (AddNode())
						beamStart = nodeManager.nodes.back();
    		    }
    		} else if (beamEnd == nullptr) {
    		    if (closestNode != nullptr && closestNode != beamStart) {
    		        beamEnd = closestNode;
    		        concludeNodeCreation();
    			}else{
					if (AddNode()){
						beamEnd = nodeManager.nodes.back();
						concludeNodeCreation();
					}
				}
			}
		}

	if (connectBeamToLastNode){
			if (beamStart == nullptr) {
				beamStart = nodeManager.nodes.back();
			} else if (beamEnd == nullptr) {
				beamEnd = nodeManager.nodes.back();
				concludeNodeCreation();
			}
		}
	}

	void AddSupport(Support supportType){
		closestNode = nodeManager.FindClosestNode(worldPosition, cameraController.camera);
		drawCursorOnClosestNode = true;	
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && closestNode){
			closestNode->support = supportType;
		}
	}

	void AddPointLoad(){
		closestNode = nodeManager.FindClosestNode(worldPosition, cameraController.camera);
		drawCursorOnClosestNode = true;	
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && closestNode){
			closestNode->AddLoad(topMenu->force, topMenu->moment);
		}
	}

	void RemPointLoad(){
		closestNode = nodeManager.FindClosestNode(worldPosition, cameraController.camera);
		drawCursorOnClosestNode = true;	
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && closestNode){
			closestNode->AddLoad(Vector2{0, 0}, 0);
		}
	}

	void RunAnalysis(){
		if (!isAnalysisUpToDate){
			std::cout << "FAZ ANALISE!" << std::endl;
			analysis.Run(&nodeManager.nodes, &beamManager.beams);
			isAnalysisUpToDate = true;
		}
	}

	void Setup() override
	{

	}

	void Shutdown() override
	{

	}

	void Show() override
	{
		beamManager.DrawShadows(cameraController.camera);
		if (isGridOn) {
            DrawBackgroundGrid(cameraController.camera);
        }

		if (isGuideOn)
			nodeGuideManager.DrawGuides(cameraController.camera);

		beamManager.Draw(cameraController.camera);





		nodeManager.DrawNodes(cameraController.camera);
		if (nodeGuideManager.drawNodeGuideSelector){
			drawSelector(nodeGuideManager.selectorToDraw.center, nodeGuideManager.selectorToDraw.camera, nodeGuideManager.selectorToDraw.color);
		}
		if (nodeGuideManager.drawDistanceSelector){
			for (const auto& selector : nodeGuideManager.selectors) {
            	drawSelector(selector.center, selector.camera, selector.color);
			}
		}

		nodeManager.DrawLoads(cameraController.camera);
		
		if (isAnalysisUpToDate){
			analysis.DrawReactions(cameraController.camera);
		}

		if (drawCursorOnClosestNode){
			if (closestNode){
				drawSelector(closestNode->position, cameraController.camera, Color{255, 255, 0, 255});
			}
		}

		if (beamStart){
			drawSelector(beamStart->position, cameraController.camera, Color{255, 255, 0, 255});
		}
		
		
    	
    	int textWidth = MeasureText(positionText.c_str(), 20);
    	DrawText(positionText.c_str(), GetScreenWidth() - textWidth - 10, GetScreenHeight() - 30, 20, WHITE);
		

		
	}

	void Update() override
	{
		mousePosition = GetMousePosition();
		worldPosition = GetScreenToWorld2D(mousePosition, cameraController.camera);

		drawCursorOnClosestNode = false;



		cameraController.UpdateCamera();


		worldGridPosition = worldPosition = GetScreenToWorld2D(mousePosition, cameraController.camera);
		if (isGridOn)
			worldGridPosition = RoundPositionToGrid(worldPosition, CalculateSpacing(baseSpacing, cameraController.camera.zoom));
		
		

		// Adiciona o nó com a distancia da distanceInputWindow
		connectBeamToLastNode = false;
		if (distanceInputWindow.createNode){
			distanceInputWindow.createNode = false;


			if (distanceInputWindow.isX){ // Se for distancia em x
				nodeManager.AddNode(Vector2{distanceInputWindow.nodeWorldPosition.x, distanceInputWindow.nodeWorldPosition.y + distanceInputWindow.distance});
			}else{ // Se for distancia em y
				nodeManager.AddNode(Vector2{distanceInputWindow.nodeWorldPosition.x + distanceInputWindow.distance, distanceInputWindow.nodeWorldPosition.y});
			}
			connectBeamToLastNode = true;
		}
		


		nodeGuideManager.drawDistanceSelector = false;
		nodeGuideManager.selectors.clear();


		if (ToolModeChanged){
			clear(toolMode);
		}
		switch (toolMode) {
		    case ToolMode::None:
		        break;

			case ToolMode::NodeAdd:
				AddNode();
				break;

			case ToolMode::NodeRem:
				RemNode();
				break;

			case ToolMode::BeamAdd:
				AddBeam();				
				break;
				
			case ToolMode::SupportNone:
				AddSupport(Support::None);				
				break;	

			case ToolMode::SupportLx:
				AddSupport(Support::Lx);				
				break;	

			case ToolMode::SupportLy:
				AddSupport(Support::Ly);				
				break;	

			case ToolMode::SupportLxy:
				AddSupport(Support::Lxy);				
				break;

			case ToolMode::SupportTv:
				AddSupport(Support::Tv);				
				break;	

			case ToolMode::SupportTh:
				AddSupport(Support::Th);				
				break;

			case ToolMode::PointLoadAdd:
				AddPointLoad();				
				break;	

			case ToolMode::PointLoadRem:
				RemPointLoad();				
				break;

			case ToolMode::ShowNormal:
				RunAnalysis();
				break;

			case ToolMode::ShowShear:
				RunAnalysis();
				break;

			case ToolMode::ShowBending:
				RunAnalysis();
				break;


			default:
		        std::cout << "Modo desconhecido" << std::endl;
		        break;
		}		
		



		if (isGuideOn)
			nodeGuideManager.UpdateGuides(&nodeManager.nodes, GetScreenToWorld2D(GetMousePosition(), cameraController.camera), cameraController.camera);
		


		std::ostringstream positionStream;
		positionStream << "Pos: (" << std::fixed << std::setprecision(3) << worldGridPosition.x << ", " << worldGridPosition.y << ")";
    	positionText = positionStream.str();
	}
	template <class Archive>
    void serialize(Archive& ar) {
        ar(
            cereal::make_nvp("nodeManager", nodeManager),
			cereal::make_nvp("materialManager", *materialManager),
			cereal::make_nvp("sectionlManager", *sectionManager),
            cereal::make_nvp("beamManager", beamManager),
			cereal::make_nvp("cameraController", cameraController)
        );
    }
};


void saveEditor(const EditorWindow& editor, const std::string& filename) {
    std::ofstream os(filename);
    cereal::JSONOutputArchive archive(os);
    archive(cereal::make_nvp("editor", editor));
}

void loadEditor(EditorWindow& editor, const std::string& filename) {
    std::ifstream is(filename);
    if (!is.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;  // Este return está aqui apenas para sair da função caso o arquivo não possa ser aberto
    }
    cereal::JSONInputArchive archive(is);
    archive(cereal::make_nvp("editor", editor));
	
	editor.beamManager.UpdateBeams(); // IMPORTANTE!!!
}


EditorWindow editor(&topMenu, &materialManager, &sectionManager);

void DoMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Arquivo"))
		{
			if (ImGui::MenuItem("Salvar")) {
                saveEditor(editor, "editor.json");
            }
            if (ImGui::MenuItem("Carregar")) {
				editor.clear(ToolMode::None);
                loadEditor(editor, "editor.json");
				isAnalysisUpToDate = false;
            }
			if (ImGui::MenuItem("Sair"))
				Quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Janela"))
		{
			ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);


			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


bool pauseEditor(){
	bool isMouseOutOfEditor = (!IsMouseOverRec(GetMousePosition(), leftMenuSize, GetScreenWidth() - rightMenuSize, topPadding, GetScreenHeight() - bottomMenuSize));
	bool pauseTheEditor = (editor.distanceInputWindow.isDistanceWindowOn || topMenu.isBeingInteracted || isMouseOutOfEditor);
	if ((toolMode == ToolMode::ShowNormal || toolMode == ToolMode::ShowShear || toolMode == ToolMode::ShowBending) && !isAnalysisUpToDate){
		pauseTheEditor = false;
	}
	return (pauseTheEditor);
}



int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1900;
	int screenHeight = 900;

	
	

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Ftool Knockoff");
	SetExitKey(KEY_NULL);
	SetTargetFPS(244);
	rlImGuiBeginInitImGui();
    ImGui::StyleColorsLight();

    ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("resources/segoeuisl.ttf", 18);
	fontTtf = LoadFontEx("resources/seguisb.ttf", myFontSize, 0, 250);
	//fontTtf = LoadFontEx("resources/consola.ttf", fontsize, 0, 250);

    rlImGuiEndInitImGui();

	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;


	topMenu.Setup();
	topMenu.Open = true;
	toolsMenu.Setup();
	toolsMenu.Open = true;
	editor.Setup();
	editor.Open = true;

	

	// Main game loop
	while (!WindowShouldClose() && !Quit)    // Detect window close button or ESC key
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);
		DrawFPS(leftMenuSize, topPadding);
		
		
		/*════════════════════════ UPDATE ════════════════════════*/
		topMenu.Update();
		toolsMenu.Update();
		if (!pauseEditor())
			editor.Update();
		



		/*═════════════════════════ DRAW ═════════════════════════*/
		if (editor.Open)
			editor.Show();
		rlImGuiBegin();

		ImGui::PushFont(font);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
		DoMainMenu();

		if (ImGuiDemoOpen)
			ImGui::ShowDemoWindow(&ImGuiDemoOpen);


		if (toolsMenu.Open)
			toolsMenu.Show();
		if (topMenu.Open)
			topMenu.Show();

		editor.distanceInputWindow.draw();
		/*════════════════════════════════════════════════════════*/
		
		ImGui::PopStyleVar();
		ImGui::PopFont();
		
		rlImGuiEnd();

		EndDrawing();
	}
	rlImGuiShutdown();

	toolsMenu.Shutdown();
	editor.Shutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}