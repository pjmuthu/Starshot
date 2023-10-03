#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "camera.h"
#include "map.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float engine;
    int numPoints;
    Vector2 path[MAX_PATH_POINTS];
    Planet collisionObject;
    float fuelMax;
    float fuel;
    int science;
    float sensor;
    float sensorMax;
    float sensorRange;
    float relativeVelocity;
    Vector2 sensorTargetPosition;
    float sensorTargetDistance;
    Vector2 pathHistory[65536];
    int pathHistoryCount;
    int pathHistoryCountRecord;
} Player;


void InitPlayer(Player* player, Sun* sun, float * time);
void UpdatePlayer(Player* player, Sun* sun, const CameraState* cameraState, float * time, int playSpeed);
void ApplyEngine(Player* player, float acceleration, Vector2 direction);

#endif // PLAYER_H
