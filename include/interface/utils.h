#pragma once
#include <raylib.h>
#include <raymath.h>
#include <cstring>
#include <cstdio>
#include "eigenpch.h"
#include "interface/globals.h"


class DocumentWindow
{
public:
	bool Open = false;


	virtual void Setup() = 0;
	virtual void Shutdown() = 0;
	virtual void Show() = 0;
	virtual void Update() = 0;

	bool Focused = false;

	Rectangle ContentRect = { 0 };
};

Vector2 RoundPositionToGrid(Vector2 position, float spacing) {
    position.x = round(position.x / spacing) * spacing;
    position.y = round(position.y / spacing) * spacing;
    return position;
}

bool IsMouseOverRec(Vector2 mousePosition, int xi, int xf, int yi, int yf){
	return (mousePosition.x > xi && mousePosition.x < xf &&
			mousePosition.y > yi && mousePosition.y < yf);
}

Vector2 RotatePoint(Vector2 pivot, Vector2 point, float angle) {
    float s = sin(angle);
    float c = cos(angle);

    // Translate point back to origin
    point.x -= pivot.x;
    point.y -= pivot.y;

    // Rotate point
    float xnew = point.x * c - point.y * s;
    float ynew = point.x * s + point.y * c;

    // Translate point back
    point.x = xnew + pivot.x;
    point.y = ynew + pivot.y;
    return point;
}

float EaseInOutQuad(float t) {
    return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}


Vector2 GetMouseWorldPosition2D(const Camera2D& camera) {
    Vector2 mousePos = GetMousePosition();
    return GetScreenToWorld2D(mousePos, camera);
}

struct SelectorToDraw {
    Vector2 center;  // Posição central do elemento visual
    Camera2D camera; // Câmera associada ao elemento visual
    Color color;     // Cor do elemento visual

    // Construtor padrão
    SelectorToDraw() = default;

    // Construtor com parâmetros
    SelectorToDraw(Vector2 c, Camera2D cam, Color col) : center(c), camera(cam), color(col) {}

};

void drawSelector(Vector2 center, Camera2D camera, Color color){
	float cursorSize = 25;
	float cursorGap = 7;
	float cursorWidth = 3;
	
	Vector2 v1 = {(center.x - (cursorSize / 2) / camera.zoom), (center.y + (cursorSize / 2) / camera.zoom)};
	Vector2 v2 = {(center.x - (cursorGap / 2) / camera.zoom), (center.y + (cursorSize / 2) / camera.zoom)};
	Vector2 v3 = {(center.x - (cursorGap / 2) / camera.zoom), (center.y + (cursorSize / 2 - cursorWidth) / camera.zoom)};
	Vector2 v4 = {(center.x - (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y + (cursorSize / 2 - cursorWidth) / camera.zoom)};
	Vector2 v5 = {(center.x - (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y + (cursorGap / 2) / camera.zoom)};
	Vector2 v6 = {(center.x - (cursorSize / 2) / camera.zoom), (center.y + (cursorGap / 2) / camera.zoom)};
	
	BeginMode2D(camera);
	DrawTriangle(v1, v2, v3, color); 
	DrawTriangle(v1, v3, v4, color); 
	DrawTriangle(v1, v4, v5, color); 
	DrawTriangle(v1, v5, v6, color); 
	EndMode2D();

	v1 = {(center.x + (cursorSize / 2) / camera.zoom), (center.y + (cursorSize / 2) / camera.zoom)};
	v2 = {(center.x + (cursorGap / 2) / camera.zoom), (center.y + (cursorSize / 2) / camera.zoom)};
	v3 = {(center.x + (cursorGap / 2) / camera.zoom), (center.y + (cursorSize / 2 - cursorWidth) / camera.zoom)};
	v4 = {(center.x + (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y + (cursorSize / 2 - cursorWidth) / camera.zoom)};
	v5 = {(center.x + (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y + (cursorGap / 2) / camera.zoom)};
	v6 = {(center.x + (cursorSize / 2) / camera.zoom), (center.y + (cursorGap / 2) / camera.zoom)};
	
	BeginMode2D(camera);
	DrawTriangle(v3, v2, v1, color); 
	DrawTriangle(v4, v3, v1, color); 
	DrawTriangle(v5, v4, v1, color); 
	DrawTriangle(v6, v5, v1, color); 
	EndMode2D();

	v1 = {(center.x - (cursorSize / 2) / camera.zoom), (center.y - (cursorSize / 2) / camera.zoom)};
	v2 = {(center.x - (cursorGap / 2) / camera.zoom), (center.y - (cursorSize / 2) / camera.zoom)};
	v3 = {(center.x - (cursorGap / 2) / camera.zoom), (center.y - (cursorSize / 2 - cursorWidth) / camera.zoom)};
	v4 = {(center.x - (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y - (cursorSize / 2 - cursorWidth) / camera.zoom)};
	v5 = {(center.x - (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y - (cursorGap / 2) / camera.zoom)};
	v6 = {(center.x - (cursorSize / 2) / camera.zoom), (center.y - (cursorGap / 2) / camera.zoom)};
	
	BeginMode2D(camera);
	DrawTriangle(v3, v2, v1, color); 
	DrawTriangle(v4, v3, v1, color); 
	DrawTriangle(v5, v4, v1, color); 
	DrawTriangle(v6, v5, v1, color); 
	EndMode2D();

	v1 = {(center.x + (cursorSize / 2) / camera.zoom), (center.y - (cursorSize / 2) / camera.zoom)};
	v2 = {(center.x + (cursorGap / 2) / camera.zoom), (center.y - (cursorSize / 2) / camera.zoom)};
	v3 = {(center.x + (cursorGap / 2) / camera.zoom), (center.y - (cursorSize / 2 - cursorWidth) / camera.zoom)};
	v4 = {(center.x + (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y - (cursorSize / 2 - cursorWidth) / camera.zoom)};
	v5 = {(center.x + (cursorSize / 2 - cursorWidth) / camera.zoom), (center.y - (cursorGap / 2) / camera.zoom)};
	v6 = {(center.x + (cursorSize / 2) / camera.zoom), (center.y - (cursorGap / 2) / camera.zoom)};
	
	BeginMode2D(camera);
	DrawTriangle(v1, v2, v3, color); 
	DrawTriangle(v1, v3, v4, color); 
	DrawTriangle(v1, v4, v5, color); 
	DrawTriangle(v1, v5, v6, color); 
	EndMode2D();
};


void zeroRow(Eigen::SparseMatrix<double>& matrix, int rowToZero) {
    for (int j = 0; j < matrix.cols(); ++j) {
        if (matrix.coeff(rowToZero, j) != 0.0) {
            matrix.coeffRef(rowToZero, j) = 0.0;
        }
    }
}

void zeroColumn(Eigen::SparseMatrix<double>& matrix, int colToZero) {
    for (int i = 0; i < matrix.rows(); ++i) {
        if (matrix.coeff(i, colToZero) != 0.0) {
            matrix.coeffRef(i, colToZero) = 0.0;  // Definir o elemento como zero
        }
    }
}

Vector2 FindIntersection(Vector2 rectSize, Vector2 size) {
    if (size.x == 0){
        return {0, std::copysign(rectSize.y / 2, size.y)};
    } else if (size.y == 0) { 
        return {std::copysign(rectSize.x / 2, size.x), 0};
        
    }

    float intersectionY = rectSize.x / 2 * fabs(size.y) / fabs(size.x);
    float intersectionLimitY = rectSize.y / 2;

    if (intersectionY < intersectionLimitY){
        return {std::copysign(rectSize.x/2, size.x), std::copysign(intersectionY, size.y)};
    } else if (intersectionY > intersectionLimitY){
        return {std::copysign((rectSize.y/2) / (fabs(size.y) / fabs(size.x)), size.x), std::copysign(rectSize.y / 2, size.y)};
    }else{
        return {std::copysign(rectSize.x / 2, size.x), std::copysign(rectSize.y / 2, size.y)};
    }
}

void drawArrow(Vector2 position, Vector2 size, bool inward, float arrowLength, float arrowWidth, float lineWidth, Color color, Camera2D camera, const char* annotation = ""){ 
    Vector2 sizeFliped = Vector2{size.x, -size.y}; // Inverte por que o raylib desenha positivo para baixo
    Vector2 endPosition;
    Vector2 p1;
    Vector2 p2;
    Vector2 p3; 

    float angle = atan2(sizeFliped.y, sizeFliped.x);


    if (inward) {
        endPosition = {position.x - sizeFliped.x, position.y - sizeFliped.y};
        p1 = {position.x, position.y};
        p2 = {position.x - arrowLength, position.y - arrowWidth};
        p3 = {position.x - arrowLength, position.y + arrowWidth};

        p2 = RotatePoint(p1, p2, angle);
        p3 = RotatePoint(p1, p3, angle);
    }else{
        endPosition = {position.x + sizeFliped.x, position.y + sizeFliped.y};
        p1 = {position.x, position.y};
        p2 = {position.x - arrowLength, position.y - arrowWidth};
        p3 = {position.x - arrowLength, position.y + arrowWidth};

        p2 = RotatePoint(p1, p2, angle);
        p3 = RotatePoint(p1, p3, angle);

        p1 = Vector2Add(p1, sizeFliped);
        p2 = Vector2Add(p2, sizeFliped);
        p3 = Vector2Add(p3, sizeFliped);
    }
    

    BeginMode2D(camera);  
    DrawLineEx(endPosition, position, lineWidth, color);
    DrawTriangle(p1, p2, p3, color);
    DrawTriangle(p3, p2, p1, color);
    EndMode2D();


    if (strcmp(annotation, "") != 0) {
        int intOffset = 3;
        if (inward)
            intOffset = - intOffset;
        Vector2 offset = Vector2Scale(Vector2Normalize(sizeFliped), intOffset / camera.zoom);
        Vector2 textSizeOri = MeasureTextEx(fontTtf, annotation, myFontSize, 1);
        Vector2 textSize = Vector2{textSizeOri.x, textSizeOri.y * .6f};
        Vector2 textPositionWorld = Vector2{endPosition.x + offset.x, endPosition.y + offset.y};
        Vector2 textPosition = Vector2Subtract(GetWorldToScreen2D(textPositionWorld, camera), Vector2Scale(textSize, .5f));
        Vector2 textCenterOffset;
        if (!inward){
            textCenterOffset = FindIntersection(textSize, size);
        }else{
            textCenterOffset = Vector2Scale(FindIntersection(textSize, size), -1);
        }
        textPosition = Vector2{textPosition.x + textCenterOffset.x, textPosition.y - textCenterOffset.y};
        //DrawRectangleLines(textPosition.x, textPosition.y, textSize.x, textSize.y, YELLOW);
	    DrawTextEx(fontTtf, annotation, Vector2{textPosition.x, textPosition.y - (textSizeOri.y - textSize.y)/2 - textSizeOri.y*.07f}, myFontSize, 1, color);

    }
}

void drawFixedSizeAnnotadedArrow(Vector2 position, Vector2 size, float scale, bool inward, float arrowLength, float arrowWidth, float lineWidth, Color color, Camera2D cam){
float module = Vector2Length(size);
float sizeInPx = module * scale;
if (sizeInPx < arrowLength)
    return;


char annotation[256];
snprintf(annotation, sizeof(annotation), "%.2f", module);


drawArrow(position, Vector2Scale(size, scale * 1 / cam.zoom), inward, arrowLength / cam.zoom, arrowWidth / cam.zoom, lineWidth / cam.zoom, color, cam, annotation);

}

void drawMoment(Vector2 position, float radius, bool isAnticlockwise, float arrowLength, float arrowWidth, float lineWidth, Color color, Camera2D camera, const char* annotation = ""){
    Vector2 p1;
    Vector2 p2;
    Vector2 p3;
    int nSteps = 18;
    float arcBegin = .4f * 2 * PI;
    float arcEnd= .1f * 2 * PI;


    std::vector<float> x(nSteps + 1);
    std::vector<float> y(nSteps + 1);

    float angleStep = (arcEnd - arcBegin) / (nSteps);

    for (int i = 0; i < nSteps + 1; ++i) {
        float angle = arcBegin + i * angleStep;
        x[i] = position.x + radius * cos(angle);
        y[i] = position.y + radius * sin(angle);
    }

    float xS;
    float yS;
    float angleS;

    if (isAnticlockwise){
        xS = x.back();
        yS = y.back();
        angleS = arcBegin + PI;
    }else{
        xS = x.front();
        yS = y.front();
        angleS = arcEnd + PI;
    }

    p1 = {xS, yS};
    p2 = {xS - arrowLength, yS - arrowWidth};
    p3 = {xS - arrowLength, yS + arrowWidth};


    p2 = RotatePoint(p1, p2, angleS);
    p3 = RotatePoint(p1, p3, angleS);


    BeginMode2D(camera);  
    for (int i = 0; i < nSteps; ++i) {
        Vector2 startPoint = {x[i], y[i]};
        Vector2 endPoint = {x[i + 1], y[i + 1]};
        DrawLineEx(startPoint, endPoint, lineWidth, color);
    }
    DrawTriangle(p1, p2, p3, color);
    DrawTriangle(p3, p2, p1, color);
    EndMode2D();

    
    if (strcmp(annotation, "") != 0) {
        int intOffset = 3;
        Vector2 offset = Vector2Scale(Vector2{0, 1}, intOffset / camera.zoom);
        Vector2 textSizeOri = MeasureTextEx(fontTtf, annotation, myFontSize, 1);
        Vector2 textSize = Vector2{textSizeOri.x, textSizeOri.y * .6f};
        Vector2 textPositionWorld = Vector2{position.x + offset.x, position.y + radius + offset.y};
        Vector2 textPosition = Vector2Subtract(GetWorldToScreen2D(textPositionWorld, camera), Vector2Scale(textSize, .5f));
        Vector2 textCenterOffset = FindIntersection(textSize, Vector2{0, -radius / camera.zoom}); // Não entendi o por que do sinal de menos
        textPosition = Vector2{textPosition.x + textCenterOffset.x, textPosition.y - textCenterOffset.y};
        //DrawRectangleLines(textPosition.x, textPosition.y, textSize.x, textSize.y, YELLOW);
	    DrawTextEx(fontTtf, annotation, Vector2{textPosition.x, textPosition.y - (textSizeOri.y - textSize.y)/2 - textSizeOri.y*.07f}, myFontSize, 1, color);

    }
}


void drawFixedSizeAnnotadedMoment(Vector2 position, float moment, float radius, float arrowLength, float arrowWidth, float lineWidth, Color color, Camera2D cam){
if (fabsf(moment) < 0.0000000001f)
    return;


char annotation[256];
snprintf(annotation, sizeof(annotation), "%.2f", fabsf(moment));

drawMoment(position, radius / cam.zoom, moment > 0 ? true : false, arrowLength / cam.zoom, arrowWidth / cam.zoom, lineWidth / cam.zoom, color, cam, annotation);
}


void colorSelector(){
    static ImVec4 imNormalFill    = rlImGuiColors::Convert(normalFill);
	static ImVec4 imNormalBorder  = rlImGuiColors::Convert(normalBorder);
	static ImVec4 imShearFill     = rlImGuiColors::Convert(shearFill);
	static ImVec4 imShearBorder   = rlImGuiColors::Convert(shearBorder);
	static ImVec4 imBendingFill   = rlImGuiColors::Convert(bendingFill);
	static ImVec4 imBendingBorder = rlImGuiColors::Convert(bendingBorder);
	bool OpenColor = true;
	if (ImGui::Begin("Cores", &OpenColor))
	{
		ImGui::SetNextItemWidth(253);
		ImGui::ColorPicker4("##1", (float*)&imNormalFill, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB);
		normalFill = rlImGuiColors::Convert(imNormalFill);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(253);
		ImGui::ColorPicker4("##2", (float*)&imNormalBorder, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB);
		normalBorder = rlImGuiColors::Convert(imNormalBorder);				
		ImGui::SameLine();

		ImGui::SetNextItemWidth(253);
		ImGui::ColorPicker4("##3", (float*)&imShearFill, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB);				
		shearFill = rlImGuiColors::Convert(imShearFill);				
		ImGui::SameLine();

		ImGui::SetNextItemWidth(253);
		ImGui::ColorPicker4("##4", (float*)&imShearBorder, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB);				
		shearBorder = rlImGuiColors::Convert(imShearBorder);				
		ImGui::SameLine();

		ImGui::SetNextItemWidth(253);
		ImGui::ColorPicker4("##5", (float*)&imBendingFill, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB);				
		bendingFill = rlImGuiColors::Convert(imBendingFill);				
		ImGui::SameLine();

		ImGui::SetNextItemWidth(253);
		ImGui::ColorPicker4("##6", (float*)&imBendingBorder, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB);				
		bendingBorder = rlImGuiColors::Convert(imBendingBorder);				
		ImGui::SameLine();
	}
	ImGui::End();
}