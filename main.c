#include <raylib.h>
#include <raymath.h>

#include "camera.h" // Include the camera header

#define MOON_MAX 8
#define PLANET_MAX 16

typedef struct {
	char name[16];
	float radius;
	float mass;
	float angle;
	float period;
	float distance;
	Vector2 position;
} Moon;

typedef struct {
	char name[16];
	float radius;
	float mass;
	float angle;
	float period;
	float distance;
	int moonCount;
	Moon moons[MOON_MAX];
	Vector2 position;
} Planet;

typedef struct {
	char name[16];
	float radius;
	float mass;
	int planetCount;
	Planet planets[PLANET_MAX];
} Sun;

Sun ourSun = {
	"Solis", 125, 220000000.0, 4,
	{
		{"Nova",   15.0, 0.260, 30.0, 20.0, 350,0},
		{"Aurora", 45.0, 0.120, 15.0, 100.0, 800.0, 1, {{"Luna", 12.0, 0.010, 45.0, -10.0, 100.0}}},
		{"Luminar", 105.0, 0.560, 90.0 , 300.0, 2400.0, 2, {{"Luna", 12.0, 0.010, -45.0, 10.0, 150.0} , {"Stella", 17.0, 0.030, 45, 20.0, 300.0}}}
	}
};


void updatePositions(Sun* sun, float t) {
	for (int i = 0; i < sun->planetCount; i++) {
		Planet* planet = &(sun->planets[i]);

		// Update planet's position
		float angle = fmod(planet->angle + (t / planet->period), 2 * PI);
		planet->position.x = cosf(angle) * planet->distance;
		planet->position.y = sinf(angle) * planet->distance;

		// Update moon positions
		for (int j = 0; j < planet->moonCount; j++) {
			Moon* moon = &(planet->moons[j]);

			angle = fmod(moon->angle + (t / moon->period), 2 * PI);
			moon->position.x = planet->position.x + cosf(angle) * moon->distance;
			moon->position.y = planet->position.y + sinf(angle) * moon->distance;
		}
	}
}

// Function to check if a point is within the camera's view
bool IsPointInCameraView(Camera2D camera, Vector2 point) {
	// Calculate the camera's visible area
	float left = camera.target.x - (camera.offset.x / camera.zoom);
	float right = left + (GetScreenWidth() / camera.zoom);
	float top = camera.target.y - (camera.offset.y / camera.zoom);
	float bottom = top + (GetScreenHeight() / camera.zoom);
	// Check if the point is within the visible area
	if (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom) {
		return true;
	}

	return false;
}

int main() {
	// Initialization
	int screenWidth = 1280;
	int screenHeight = 720;
	//SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Starshot");

	CameraState cameraState = InitCameraState(screenWidth, screenHeight);


	// Main loop
	float time = 0.0f;
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		time += 0.1f;  // Increment time by 1 day per frame

		UpdateCameraState(&cameraState);
		updatePositions(&ourSun, time);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(cameraState.camera);
		if (IsPointInCameraView(cameraState.camera, Vector2Zero())) {
			DrawCircleV(Vector2Zero(), ourSun.radius, GRAY);
		}
		// Draw planet and moon positions
		float skew = cameraState.skew;
		for (int i = 0; i < ourSun.planetCount; i++) {
			DrawEllipseLines(0, 0, ourSun.planets[i].distance, ourSun.planets[i].distance / skew, LIGHTGRAY);
			Vector2 planetPosition = { ourSun.planets[i].position.x, ourSun.planets[i].position.y / skew };
			if (IsPointInCameraView(cameraState.camera, planetPosition)) {
				DrawCircleV(planetPosition, ourSun.planets[i].radius, GRAY);
				for (int j = 0; j < ourSun.planets[i].moonCount; j++) {
					Vector2 moonPosition = { ourSun.planets[i].moons[j].position.x, ourSun.planets[i].moons[j].position.y / skew };
					if (IsPointInCameraView(cameraState.camera, moonPosition)) {
						DrawCircleV(moonPosition, ourSun.planets[i].moons[j].radius, GRAY);
					}
				}
			}
		}

		EndMode2D();
		DrawFPS(10, 10);
		EndDrawing();
	}

	// De-Initialization
	CloseWindow();

	return 0;
}
