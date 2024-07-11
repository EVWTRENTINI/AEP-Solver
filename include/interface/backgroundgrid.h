#pragma once


float CalculateSpacing(float baseSpacing, float zoomLevel) {
    float rawSpacing = baseSpacing / zoomLevel;
    float exponent = floor(log10(rawSpacing));
    float normalizedSpacing = pow(10, exponent);

    // Ajuste para espaçamentos menores que 1
    if (rawSpacing / normalizedSpacing < 1.0) {
        normalizedSpacing /= 10;
    }

    return normalizedSpacing;
}

void DrawBackgroundGrid(Camera2D camera) {
    // Calcule o espaçamento baseado no zoom da camera
    float baseSpacing = 100.0f; // Espaçamento base
    float zoomLevel = camera.zoom;
    float spacing = CalculateSpacing(baseSpacing, zoomLevel);

    // Calcule os limites da tela em coordenadas do mundo
    Vector2 worldTopLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
    Vector2 worldBottomRight = GetScreenToWorld2D(Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);

    // Desenhar a camada secundária (linhas mais fracas)
    float secondarySpacing = spacing;
    Color secondaryColor = Color{ 0, 0, 0, 25};

    float startX = worldTopLeft.x - fmod(worldTopLeft.x, secondarySpacing);
    float startY = worldTopLeft.y - fmod(worldTopLeft.y, secondarySpacing);
	BeginMode2D(camera);
    for (float x = startX; x < worldBottomRight.x; x += secondarySpacing) {
        // Desenhe a linha secundária apenas se não estiver em uma linha primária
        if (fmod(x, spacing * 5) != 0) {
            DrawLineEx(Vector2{x, worldTopLeft.y}, Vector2{x, worldBottomRight.y}, 1.0f / camera.zoom, secondaryColor);
        }
    }

    for (float y = startY; y < worldBottomRight.y; y += secondarySpacing) {
        // Desenhe a linha secundária apenas se não estiver em uma linha primária
        if (fmod(y, spacing * 5) != 0) {
            DrawLineEx(Vector2{worldTopLeft.x, y}, Vector2{worldBottomRight.x, y}, 1.0f / camera.zoom, secondaryColor);
        }
    }

    // Desenhar a camada primária (linhas mais fortes)
    float primarySpacing = spacing * 5.0f;
    Color primaryColor = Color{ 0, 0, 0, 50};

    startX = worldTopLeft.x - fmod(worldTopLeft.x, primarySpacing);
    startY = worldTopLeft.y - fmod(worldTopLeft.y, primarySpacing);

    for (float x = startX; x < worldBottomRight.x; x += primarySpacing) {
        DrawLineEx(Vector2{x, worldTopLeft.y}, Vector2{x, worldBottomRight.y}, 2.0f / camera.zoom, primaryColor);
    }

    for (float y = startY; y < worldBottomRight.y; y += primarySpacing) {
        DrawLineEx(Vector2{worldTopLeft.x, y}, Vector2{worldBottomRight.x, y}, 2.0f / camera.zoom, primaryColor);
    }

    EndMode2D();
}