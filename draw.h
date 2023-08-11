#ifndef DRAW_H
#define DRAW_H

#include <raylib.h>
#include <raymath.h>
#include "camera.h"
#include "map.h"
#include "player.h"

void DrawMap(const CameraState* cameraState, Sun ourSun, float time);
void DrawPlayer(Player* player, const CameraState* cameraState);

#endif // DRAW_H
