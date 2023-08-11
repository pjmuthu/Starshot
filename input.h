#ifndef INPUT_H
#define INPUT_H

#include "player.h"
#include "camera.h"

void HandleKeyboardInput(Player* player, Sun* sun, CameraState* cameraState, float* time);
void HandleMouseInput(Player* player,Sun* sun,  CameraState* cameraState, float* time);

#endif // INPUT_H
