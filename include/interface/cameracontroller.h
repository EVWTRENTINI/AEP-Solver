#pragma once
#include "raylib.h"
#include "raymath.h"

class CameraController {
public:
	Camera2D camera;
	float bZoom;
	float r = .0000001f;

	CameraController(){
		camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.target = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 100.0f;

		bZoom = camera.zoom;
	}

	void UpdateCamera(){
		// Translate based on mouse right click
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f / camera.zoom);
			camera.target = Vector2Add(camera.target, delta);
		}
		if (IsWindowResized())
		{
			camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
		}
		// Zoom based on mouse wheel
		float wheel = GetMouseWheelMove();
		if (wheel != 0)	{
			// Get the world point that is under the mouse
			Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

			// Set the offset to where the mouse is
			camera.offset = GetMousePosition();

			// Set the target to match, so that the camera maps the world space point
			// under the cursor to the screen space point under the cursor at any zoom
			camera.target = mouseWorldPos;

			// Zoom increment
			float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
			if (wheel < 0)
				scaleFactor = 1.0f / scaleFactor;
			bZoom = Clamp(bZoom * scaleFactor, 1.0f, 100000.0f);
			
		}
		camera.zoom = (camera.zoom-bZoom)*pow(r,GetFrameTime())+bZoom;
	}
	template <class Archive>
    void serialize(Archive& ar) {
        ar(
            cereal::make_nvp("camera", camera),
            cereal::make_nvp("bZoom", bZoom)
        );
    }
};