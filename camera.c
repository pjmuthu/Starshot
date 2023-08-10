#include "camera.h"
#include "raymath.h"


CameraState InitCameraState(int screenWidth, int screenHeight) {
	CameraState cameraState;
	cameraState.camera.target = (Vector2){ 0, 0 };
	cameraState.camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
	cameraState.camera.rotation = 0.0f;
	cameraState.camera.zoom = 1.0f;
	cameraState.velocity = (Vector2){ 0, 0 };
	cameraState.zoomVelocity = 0.0f;
	return cameraState;
}

void UpdateCameraState(CameraState* cameraState) {
	if (IsWindowResized())	{
		cameraState->camera.offset = (Vector2){ GetScreenWidth() / 2, GetScreenHeight() / 2 };
	}

	if (IsKeyDown(KEY_W)) cameraState->velocity.y -= 0.1f / cameraState->camera.zoom;
	if (IsKeyDown(KEY_S)) cameraState->velocity.y += 0.1f / cameraState->camera.zoom;
	if (IsKeyDown(KEY_A)) cameraState->velocity.x -= 0.1f / cameraState->camera.zoom;
	if (IsKeyDown(KEY_D)) cameraState->velocity.x += 0.1f / cameraState->camera.zoom;

	// Apply velocity to camera target
	cameraState->camera.target.x += cameraState->velocity.x;
	cameraState->camera.target.y += cameraState->velocity.y;

	// Apply acceleration to zooming
	float zoomAcceleration = ((float)GetMouseWheelMove() * 0.01f);
	cameraState->zoomVelocity += zoomAcceleration;

	// Apply velocity to zoom level
	cameraState->camera.zoom += cameraState->zoomVelocity;

	// Limit zoom velocity to prevent excessive acceleration
	cameraState->zoomVelocity *= 0.9f;

	// Clamp zoom and velocity
	cameraState->camera.zoom = Clamp(cameraState->camera.zoom, 0.1f, 2.0f);
	cameraState->velocity = Vector2Scale(cameraState->velocity, 0.95f);

	cameraState->skew = Clamp((cameraState->camera.zoom * 2)+0.5, 1, 2);

}

void CloseCameraState(CameraState* cameraState) {
	// No need for explicit cleanup in this case
}
