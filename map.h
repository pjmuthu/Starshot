#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include "camera.h"

#define MOON_MAX 8
#define PLANET_MAX 16
#define MAX_PATH_POINTS 8192

typedef struct {
	char name[16];
	float radius;
	float angle;
	float period;
	float distance;
	Vector2 position;
} Moon;

typedef struct {
	char name[16];
	float radius;
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
	int planetCount;
	Planet planets[PLANET_MAX];
	Vector2 startPosition;
	Vector2 startVelocity;
} Sun;

static float step_time = 0.01;

void UpdateMapPositions(Sun* sun, float t);
Planet* GetClosestPlanet(Vector2 position, Sun* sun, float t);

#endif // MAP_H