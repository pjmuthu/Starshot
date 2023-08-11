#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "camera.h"
#include "map.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float acceleration;
    int numPoints;
    Vector2 path[MAX_PATH_POINTS];
    Planet collisionObject;
} Player;

void InitPlayer(Player* player, Sun* sun, float * time);
void UpdatePlayer(Player* player, Sun* sun, const CameraState* cameraState, float * time);

#endif // PLAYER_H
