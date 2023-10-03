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

	for (int i = 1; i < player->pathHistoryCountRecord; i++) {
		Vector2 position1 = { player->pathHistory[i - 1].x, player->pathHistory[i - 1].y * cameraState->tilt };
		Vector2 position2 = { player->pathHistory[i].x, player->pathHistory[i].y * cameraState->tilt };
		if (i < player->pathHistoryCount) {
			float weight = (float)i / player->pathHistoryCount;
			if (weight > 0.25) {
				DrawLineEx(position1, position2, 4 * weight / cameraState->camera.zoom, ColorAlpha(DARKGRAY, 0.5 * weight * weight));
			}
		}
		else if (i > player->pathHistoryCount) {
			float weight = (float)(i - player->pathHistoryCount) / (player->pathHistoryCountRecord - player->pathHistoryCount);
			DrawLineEx(position1, position2, 2 * weight / cameraState->camera.zoom, ColorAlpha(RED, 0.5));
		}
	}
	DrawLineEx(position, (Vector2) { player->pathHistory[player->pathHistoryCount - 1].x, player->pathHistory[player->pathHistoryCount - 1].y* cameraState->tilt }, 4 / cameraState->camera.zoom, ColorAlpha(DARKGRAY, 0.5));




	DrawEllipse(position.x, position.y, player->sensorRange, player->sensorRange * cameraState->tilt, ColorAlpha(WHITE, 0.25));
	DrawEllipseLines(position.x, position.y, player->sensorRange, player->sensorRange * cameraState->tilt, ColorAlpha(LIGHTGRAY, 0.5));


	//DrawText(TextFormat("%0.1f km/s", 32 * player->relativeVelocity), position.x - 16 / cameraState->camera.zoom, position.y - 16 / cameraState->camera.zoom, 12 / cameraState->camera.zoom, GRAY);
	DrawPolyLines(position, 3, 8 / cameraState->camera.zoom, 90.0f, GRAY);
	DrawLineStrip(player->path, player->numPoints, GRAY);
	if (player->numPoints < MAX_PATH_POINTS) {
		DrawCircleV(player->path[player->numPoints - 1], 4 / cameraState->camera.zoom, RED);
	}

	//DrawText(TextFormat("%0.0f%", Vector2Distance(player->position, player->sensorTarget->position) - player->sensorTarget->radius), (player->position.x + player->sensorTarget->position.x) / 2, (player->position.y + player->sensorTarget->position.y) / 2 * cameraState->tilt, 32, GRAY);
	//DrawText(TextFormat("%0.0f%", Vector2Distance(player->position, player->sensorTarget->position)), (player->position.x + player->sensorTarget->position.x) / 2, (player->position.y + player->sensorTarget->position.y) / 2 * cameraState->tilt, 32, GRAY);

	float fraction = player->sensor / player->sensorMax;

	if (fraction < 0.5)
	{	/*
		if (fraction < 0.5) {
			fraction = fraction / 0.5;
		}
		else {
			fraction = 1.0 - (fraction - 0.5) / 0.5;
		}

		position = Vector2Lerp(player->position, player->sensorTargetPosition, fraction);
		position.y *= cameraState->tilt;
		DrawCircleV(position, 4, LIGHTGRAY);
		*/
		DrawLine(player->position.x - 32, player->position.y * cameraState->tilt, player->sensorTargetPosition.x, player->sensorTargetPosition.y * cameraState->tilt, ColorAlpha(LIGHTGRAY, 0.5));
	}

	Rectangle fuelLevelBackground = { player->position.x - 32 / cameraState->camera.zoom, player->position.y * cameraState->tilt + 16 / cameraState->camera.zoom, 0.25 * player->fuelMax / cameraState->camera.zoom,  8 / cameraState->camera.zoom };
	Rectangle healthBar = { fuelLevelBackground.x, fuelLevelBackground.y, fuelLevelBackground.width, fuelLevelBackground.height };
	healthBar.width = 0.25 * player->fuel / cameraState->camera.zoom;
	DrawRectangleRec(healthBar, player->fuel < 64 ? CLITERAL(Color) { 214, 48, 49, 255 } : CLITERAL(Color) { 231, 187, 118, 255 });
	DrawRectangleLinesEx(fuelLevelBackground, 1.0f, LIGHTGRAY);
	DrawText(TextFormat("%0.1f%%", 100 * player->fuel / player->fuelMax), fuelLevelBackground.x, fuelLevelBackground.y + 8 / cameraState->camera.zoom, 16 / cameraState->camera.zoom, GRAY);


}


void DrawUI(Player* player, int* playspeed) {

	/*
	Rectangle sensorLevelBackground = { 64, 40, 0.5 * player->sensorMax, 16 };
	healthBar = (Rectangle){ sensorLevelBackground.x, sensorLevelBackground.y, sensorLevelBackground.width, sensorLevelBackground.height };
	healthBar.width = 0.5 * player->sensor;
	DrawRectangleRec(healthBar, GRAY);
	DrawRectangleLinesEx(sensorLevelBackground, 1.0f, LIGHTGRAY);
	DrawText(TextFormat("Snsr                       %0.1f%%", 100 * player->sensor / player->sensorMax), 25, 40, 16, GRAY);
	*/

	DrawText(TextFormat("Points: %i%", player->science), screenWidth - 120, 10, 25, GRAY);


}
