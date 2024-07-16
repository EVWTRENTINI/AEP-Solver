#pragma once
#include <raylib.h>
#include <raymath.h>
#include "eigenpch.h"

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

void drawArrow(Vector2 position, Vector2 size, Camera2D camera, bool inward, float arrowLength, float arrowWidth, float lineWidth, Color color){
    Vector2 endPosition;
    Vector2 p1;
    Vector2 p2;
    Vector2 p3;
    float angle = atan2(size.y, size.x);


    if (inward) {
        endPosition = {position.x - size.x, position.y - size.y};
        p1 = {position.x, position.y};
        p2 = {position.x - arrowLength, position.y - arrowWidth};
        p3 = {position.x - arrowLength, position.y + arrowWidth};

        p2 = RotatePoint(p1, p2, angle);
        p3 = RotatePoint(p1, p3, angle);
    }else{
        endPosition = {position.x + size.x, position.y + size.y};
        p1 = {position.x, position.y};
        p2 = {position.x - arrowLength, position.y - arrowWidth};
        p3 = {position.x - arrowLength, position.y + arrowWidth};

        p2 = RotatePoint(p1, p2, angle);
        p3 = RotatePoint(p1, p3, angle);

        p1 = Vector2Add(p1, size);
        p2 = Vector2Add(p2, size);
        p3 = Vector2Add(p3, size);
    }
    
    BeginMode2D(camera);  
    DrawLineEx(endPosition, position, lineWidth, color);
    DrawTriangle(p1, p2, p3, color);
    DrawTriangle(p3, p2, p1, color);
    EndMode2D();
}