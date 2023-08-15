#ifndef INPUT_H
#define INPUT_H

#include "player.h"
#include "camera.h"

void HandleKeyboardInput(Player* player, Sun* sun, CameraState* cameraState, float* time, int* playspeed);
void HandleMouseInput(Player* player, Sun* sun, CameraState* cameraState, float* time, int* playspeed);

#endif // INPUT_H
