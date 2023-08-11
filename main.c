#include <raylib.h>

#include "camera.h"
#include "map.h"
#include "draw.h"
#include "level.h"
#include "player.h"
#include "input.h"

int main() {
	// Initialization
	int screenWidth = 1280;
	int screenHeight = 720;
	//SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Starshot");

	CameraState cameraState = InitCameraState(screenWidth, screenHeight);

	// Main loop
	Sun ourSun = ADITYA;
	Player player;
	float time = 0;

	InitPlayer(&player, &ourSun, &time);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		time += step_time;

		HandleKeyboardInput(&player, &ourSun, &cameraState, &time);
		HandleMouseInput(&player, &ourSun , &cameraState, &time);

		UpdateCameraState(&cameraState);
		UpdatePlayer(&player, &ourSun, &cameraState, &time);
		UpdateMapPositions(&ourSun, time);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		BeginMode2D(cameraState.camera);
		DrawMap(&cameraState, ourSun, time);
		DrawPlayer(&player, &cameraState);
		EndMode2D();
		
		DrawFPS(10, 10);
		EndDrawing();

	}

	// De-Initialization
	CloseCameraState(&cameraState);
	CloseWindow();
	
	return 0;
}
