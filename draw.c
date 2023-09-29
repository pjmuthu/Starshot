#include "map.h"
#include "player.h"

#include <raymath.h>

int screenWidth = 1280;
int screenHeight = 720;

// Function to check if a point is within the camera's view
bool IsPointInCameraView(const Camera2D* camera, Vector2 point) {
	if (IsWindowResized()) {
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();
	}
	// Calculate the camera's visible area
	float left = camera->target.x - (camera->offset.x / camera->zoom);
	float right = left + (screenWidth / camera->zoom);
	float top = camera->target.y - (camera->offset.y / camera->zoom);
	float bottom = top + (screenHeight / camera->zoom);
	// Check if the point is within the visible area
	if (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom) {
		return true;
	}

	return false;
}

void DrawMap(const CameraState* cameraState, Sun ourSun, float time) {

	// Draw planet and moon positions
	for (int i = 0; i < ourSun.planetCount; i++) {
		Planet planet = ourSun.planets[i];
		DrawEllipseLines(0, 0, planet.distance, planet.distance * cameraState->tilt, LIGHTGRAY);
		Vector2 planetPosition = { planet.position.x, planet.position.y * cameraState->tilt };
		if (IsPointInCameraView(&(cameraState->camera), planetPosition)) {
			DrawCircleV(planetPosition, planet.radius + 1 / cameraState->camera.zoom, DARKGRAY);
			DrawCircleV(planetPosition, planet.radius, LIGHTGRAY);
			float fontSize = 16 / cameraState->camera.zoom;
			for (int j = 0; j < planet.health; j++) {
				Rectangle recPlanet = { planetPosition.x + planet.radius - j * 20, planetPosition.y - planet.radius - 10, 16 , 16 };
				DrawRectangleRec(recPlanet, planet.armor > j ? DARKGRAY : LIGHTGRAY);
				DrawRectangleLinesEx(recPlanet, 1 / cameraState->camera.zoom, DARKGRAY);
			}
			for (int j = 0; j < planet.moonCount; j++) {
				Moon moon = planet.moons[j];
				Vector2 moonPosition = { moon.position.x, moon.position.y * cameraState->tilt };
				DrawCircleV(moonPosition, moon.radius + 1 / cameraState->camera.zoom, DARKGRAY);
				DrawCircleV(moonPosition, moon.radius, LIGHTGRAY);
				for (int j = 0; j < moon.health; j++) {
					Rectangle recMoon = { moonPosition.x + moon.radius - j * 20, moonPosition.y - moon.radius - 10, 16 , 16 };
					DrawRectangleRec(recMoon, moon.armor > j ? DARKGRAY : LIGHTGRAY);
					DrawRectangleLinesEx(recMoon, 1 / cameraState->camera.zoom, DARKGRAY);
				}
			}
		}
	}

	if (IsPointInCameraView(&(cameraState->camera), Vector2Zero())) {
		DrawCircleV(Vector2Zero(), ourSun.radius + 1 / cameraState->camera.zoom, DARKGRAY);
		DrawCircleV(Vector2Zero(), ourSun.radius, LIGHTGRAY);
	}
}

void DrawPlayer(Player* player, const CameraState* cameraState) {
	Vector2 position = { player->position.x, player->position.y * cameraState->tilt };

	DrawEllipse(position.x, position.y, player->sensorRange, player->sensorRange * cameraState->tilt, ColorAlpha(WHITE, 0.25));
	DrawEllipseLines(position.x, position.y, player->sensorRange, player->sensorRange * cameraState->tilt, ColorAlpha(LIGHTGRAY, 0.5));


	DrawText(TextFormat("%0.1f km/s", 32 * player->relativeVelocity), position.x - 16 / cameraState->camera.zoom, position.y - 16 / cameraState->camera.zoom, 12 / cameraState->camera.zoom, GRAY);
	DrawPolyLines(position, 3, 8 / cameraState->camera.zoom, 90.0f, GRAY);
	DrawLineStrip(player->path, player->numPoints, GRAY);
	if (player->numPoints < MAX_PATH_POINTS) {
		DrawCircleV(player->path[player->numPoints - 1], 4 / cameraState->camera.zoom, RED);
	}
}


void DrawUI(Player* player, int* playspeed) {
	Rectangle fuelLevelBackground = { 64, 20, 0.5 * player->fuelMax, 16 };
	Rectangle healthBar = { fuelLevelBackground.x, fuelLevelBackground.y, fuelLevelBackground.width, fuelLevelBackground.height };
	healthBar.width = 0.5 * player->fuel;
	DrawRectangleRec(healthBar, player->fuel < 64 ? CLITERAL(Color) { 214, 48, 49, 255 } : CLITERAL(Color) { 231, 187, 118, 255 });
	DrawRectangleLinesEx(fuelLevelBackground, 1.0f, LIGHTGRAY);
	DrawText(TextFormat("Fuel                        %0.1f%%", 100 * player->fuel / player->fuelMax), 25, 20, 16, GRAY);


	Rectangle sensorLevelBackground = { 64, 40, 0.5 * player->sensorMax, 16 };
	healthBar = (Rectangle) { sensorLevelBackground.x, sensorLevelBackground.y, sensorLevelBackground.width, sensorLevelBackground.height };
	healthBar.width = 0.5 * player->sensor;
	DrawRectangleRec(healthBar, GRAY);
	DrawRectangleLinesEx(sensorLevelBackground, 1.0f, LIGHTGRAY);
	DrawText(TextFormat("Snsr                       %0.1f%%", 100 * player->sensor / player->sensorMax), 25, 40, 16, GRAY);


}
