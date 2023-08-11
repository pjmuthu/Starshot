#include "camera.h"
#include "raymath.h"


CameraState InitCameraState(int screenWidth, int screenHeight) {
	CameraState cameraState;
	cameraState.camera.target = (Vector2){ 0, 0 };
	cameraState.camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
	cameraState.camera.rotation = 0.0f;
	cameraState.camera.zoom = 1.0f;
	cameraState.velocity = (Vector2){ 0, 0 };
	return cameraState;
}

void UpdateCameraState(CameraState* cameraState) {
	if (IsWindowResized())	{
		cameraState->camera.offset = (Vector2){ GetScreenWidth() / 2, GetScreenHeight() / 2 };
	}

	cameraState->camera.zoom = Clamp(cameraState->camera.zoom, 0.1f, 2.0f);
	cameraState->tilt = 1/Clamp((cameraState->camera.zoom * 2)+0.5, 1, 4);

}

void CloseCameraState(CameraState* cameraState) {
	// No need for explicit cleanup in this case
}
