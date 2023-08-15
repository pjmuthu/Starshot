#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "player.h"

void HandleKeyboardInput(Player* player, Sun* sun, CameraState* cameraState, float* time, int * playspeed) {
	if (IsKeyDown(KEY_W)) cameraState->camera.target.y -= 2.0f / cameraState->camera.zoom;
	else if (IsKeyDown(KEY_S)) cameraState->camera.target.y += 2.0f / cameraState->camera.zoom;
	if (IsKeyDown(KEY_A)) cameraState->camera.target.x -= 2.0f / cameraState->camera.zoom;
	if (IsKeyDown(KEY_D)) cameraState->camera.target.x += 2.0f / cameraState->camera.zoom;
	else if (IsKeyPressed(KEY_R)) InitPlayer(player, sun, time);
	if (IsKeyPressed(KEY_P)) *playspeed = 0;
	if (IsKeyPressed(KEY_KP_ADD)) if (GetFPS() > TARGET_FPS * 0.5f ) *playspeed += 1;
	if (IsKeyPressed(KEY_KP_SUBTRACT)) if (*playspeed > 0) 	*playspeed -= 1;
}

float ExponentialEaseInOut(float t) {
	if (t <= 0.0f) return 0.0f;
	if (t >= 1.0f) return 1.0f;
	return t < 0.5f ? 0.5f * pow(2.0f, 10.0f * (2.0f * t - 1.0f)) : 0.5f * (-pow(2.0f, -10.0f * (2.0f * t - 1.0f)) + 2.0f);
}

void HandleMouseInput(Player* player, Sun* sun, CameraState* cameraState, float* time, int* playspeed) {
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePositionScreen = GetMousePosition();
		Vector2 mousePositionWorld = GetScreenToWorld2D(mousePositionScreen, cameraState->camera);
		mousePositionWorld.y /= cameraState->tilt;
		Vector2 mouseToPlayer = Vector2Subtract(mousePositionWorld, player->position);
		float distance = Vector2Length(mouseToPlayer);
		mouseToPlayer = Vector2Normalize(mouseToPlayer);
		float modifiedAcceleration = Lerp(player->engine * 0.01f, player->engine, ExponentialEaseInOut(Clamp(distance / 720.0f, 0.0f, 1.0f)));
		if (*playspeed > 0) ApplyEngine(player, modifiedAcceleration, mouseToPlayer);
	}

	float targetZoom = cameraState->camera.zoom;
	const float zoomSpeed = 0.5f;
	targetZoom += GetMouseWheelMove() * zoomSpeed;
	cameraState->camera.target.y += GetMouseWheelMove() * cameraState->camera.zoom;
	targetZoom = Clamp(targetZoom, 0.1f, 2.0f);
	float zoomDiff = targetZoom - cameraState->camera.zoom;
	cameraState->camera.zoom += zoomDiff * 0.1f;
	cameraState->tilt = 1 / Clamp((cameraState->camera.zoom * 2) + 1.0, 1, 2);
}
