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

	CameraState cameraState = InitCameraState();

	// Main loop
	Sun ourSun = ADITYA;
	Player player;
	float time = 0;
	float timeStep = TIME_STEP;
	int frames = 0;
	int playspeed = 1;
	InitPlayer(&player, &ourSun, &time);

	SetTargetFPS(TARGET_FPS);
	while (!WindowShouldClose()) {
		frames++;
		time += playspeed == 0 ? 0.0f : TIME_STEP;

		UpdatePlayer(&player, &ourSun, &cameraState, &time, playspeed == 0);

		if (frames > playspeed) {
			frames = 0;
			UpdateCameraState(&cameraState);
			UpdateMapPositions(&ourSun, time);
			HandleKeyboardInput(&player, &ourSun, &cameraState, &time, &playspeed);
			HandleMouseInput(&player, &ourSun, &cameraState, &time, &playspeed);
			BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(cameraState.camera);
			DrawMap(&cameraState, ourSun, time);
			DrawPlayer(&player, &cameraState);
			EndMode2D();
			DrawUI(&player, &playspeed);
			EndDrawing();
		}
	}

	// De-Initialization
	CloseCameraState(&cameraState);
	CloseWindow();

	return 0;
}
