#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "player.h"

void HandleKeyboardInput(Player* player, Sun* sun, CameraState* cameraState, float* time) {
	if (IsKeyDown(KEY_W)) cameraState->camera.target.y -= 2.0f / cameraState->camera.zoom;
	else if (IsKeyDown(KEY_S)) cameraState->camera.target.y += 2.0f / cameraState->camera.zoom;
	if (IsKeyDown(KEY_A)) cameraState->camera.target.x -= 2.0f / cameraState->camera.zoom;
	if (IsKeyDown(KEY_D)) cameraState->camera.target.x += 2.0f / cameraState->camera.zoom;
	else if (IsKeyPressed(KEY_R)) InitPlayer(player, sun, time);

	if (IsKeyDown(KEY_UP)) player->velocity.y -= player->acceleration * 0.1f;
	else if (IsKeyDown(KEY_DOWN)) player->velocity.y += player->acceleration * 0.1f;
	if (IsKeyDown(KEY_LEFT)) player->velocity.x -= player->acceleration * 0.1f;
	else if (IsKeyDown(KEY_RIGHT)) player->velocity.x += player->acceleration * 0.1f;


}

void HandleMouseInput(Player* player, Sun* sun, CameraState* cameraState, float* time) {
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePositionScreen = GetMousePosition();
		Vector2 mousePositionWorld = GetScreenToWorld2D(mousePositionScreen, cameraState->camera);
		mousePositionWorld.y /= cameraState->tilt;
		Vector2 mouse2Player = Vector2Subtract(mousePositionWorld, player->position);
		float magnitude = Vector2LengthSqr(mouse2Player);
		mouse2Player = Vector2Normalize(mouse2Player);
		float fraction = Clamp(magnitude / 262144, 0.0f, 1.0f);
		float modifiedAcceleration = player->acceleration * fraction * fraction;
		player->velocity = Vector2Add(player->velocity, Vector2Scale(mouse2Player, modifiedAcceleration));
	} 


	cameraState->camera.zoom -= -GetMouseWheelMove() * 0.1f;
	cameraState->camera.zoom = Clamp(cameraState->camera.zoom, 0.1f, 2.0f);
	cameraState->tilt = 1 / Clamp((cameraState->camera.zoom * 2) + 0.5, 1, 2);
}
