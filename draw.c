#include "map.h"
#include "player.h"

#include <raymath.h>

// Function to check if a point is within the camera's view
bool IsPointInCameraView(const Camera2D* camera, Vector2 point) {
	// Calculate the camera's visible area
	float left = camera->target.x - (camera->offset.x / camera->zoom);
	float right = left + (GetScreenWidth() / camera->zoom);
	float top = camera->target.y - (camera->offset.y / camera->zoom);
	float bottom = top + (GetScreenHeight() / camera->zoom);
	// Check if the point is within the visible area
	if (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom) {
		return true;
	}

	return false;
}

void DrawMap(const CameraState* cameraState, Sun ourSun, float time) {

	// Draw planet and moon positions
	for (int i = 0; i < ourSun.planetCount; i++) {
		DrawEllipseLines(0, 0, ourSun.planets[i].distance, ourSun.planets[i].distance * cameraState->tilt, LIGHTGRAY);
		Vector2 planetPosition = { ourSun.planets[i].position.x, ourSun.planets[i].position.y * cameraState->tilt };
		if (IsPointInCameraView(&(cameraState->camera), planetPosition)) {
			DrawCircleV(planetPosition, ourSun.planets[i].radius, LIGHTGRAY);
			for (int j = 0; j < ourSun.planets[i].moonCount; j++) {
				Vector2 moonPosition = { ourSun.planets[i].moons[j].position.x, ourSun.planets[i].moons[j].position.y * cameraState->tilt };
				if (IsPointInCameraView(&(cameraState->camera), moonPosition)) {
					DrawCircleV(moonPosition, ourSun.planets[i].moons[j].radius, LIGHTGRAY);
				}
			}
		}
	}

	if (IsPointInCameraView(&(cameraState->camera), Vector2Zero())) {
		DrawCircleV(Vector2Zero(), ourSun.radius, LIGHTGRAY);
	}
}

void DrawPlayer(Player* player, const CameraState* cameraState) {
	Vector2 position = { player->position.x, player->position.y * cameraState->tilt };
	DrawPolyLines(position, 3, 8 / cameraState->camera.zoom, 90.0f, GRAY);
	DrawLineStrip(player->path, player->numPoints, GRAY);
	if (player->numPoints < MAX_PATH_POINTS) {
		DrawCircleV(player->path[player->numPoints - 1], 4 / cameraState->camera.zoom, RED);
	}
}


void DrawUI(Player* player, int * playspeed) {
	Rectangle fuelLevelBackground = { 20, 20, player->fuelMax, 10 };
	DrawRectangleRec(fuelLevelBackground, LIGHTGRAY);
	Rectangle healthBar = { fuelLevelBackground.x, fuelLevelBackground.y, fuelLevelBackground.width, fuelLevelBackground.height };
	healthBar.width = player->fuel;
	DrawRectangleRec(healthBar, player->fuel < 64 ? CLITERAL(Color) { 214, 48, 49, 255 } : CLITERAL(Color) { 231, 187, 118, 255 });

	DrawText(TextFormat("%2i", *playspeed), 10, 10, 10, GRAY);
	DrawText(TextFormat("%2i", GetFPS()), 30, 10, 10, GRAY);
}
