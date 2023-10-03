#include "player.h"
#include "raymath.h"
#include "map.h"


static float timeRecord;

void InitPlayer(Player* player, Sun* sun, float* time) {
	player->position = sun->startPosition;
	player->velocity = sun->startVelocity;
	player->engine = 0.0005f;
	player->numPoints = MAX_PATH_POINTS;
	player->collisionObject.moonCount = -1;
	*time = 0;
	player->fuelMax = 256;
	player->fuel = player->fuelMax;
	player->sensorMax = 256;
	player->sensor = player->sensorMax;
	player->sensorRange = 128;
	player->science = 0;
	timeRecord = 0;
	//for (int i = 0; i < player->pathHistoryCount; i++) {
	//	player->pathHistoryRecord[i] = player->pathHistory[i];
	//}
	player->pathHistoryCountRecord = player->pathHistoryCount;
	player->pathHistoryCount = 1;
	player->pathHistory[0] = sun->startPosition;

	InitMap(sun);
}

void ApplyEngine(Player* player, float acceleration, Vector2 direction) {
	player->fuel -= acceleration * 32;
	player->velocity = Vector2Add(player->velocity, Vector2Scale(direction, acceleration));
}


bool ApplyGravity(Player* player, Sun* sun, float time) {
	const int GRAVITY = 2.0;
	Vector2 sunToPlayer = Vector2Negate(player->position);
	float minDistance = Vector2LengthSqr(sunToPlayer);
	UpdateMapPositions(sun, time);
	Planet* closestPlanet = NULL;
	for (int i = 0; i < sun->planetCount; i++) {
		Planet* planet = &(sun->planets[i]);
		float distance = Vector2DistanceSqr(planet->position, player->position);
		if (distance < minDistance) {
			minDistance = distance;
			closestPlanet = planet;
		}
	}
	if (closestPlanet == NULL) {
		if (minDistance < (sun->radius * sun->radius)) return false;
		float forceMagnitude = sun->radius / minDistance;
		float forceDirection = atan2f(sunToPlayer.y, sunToPlayer.x);
		player->velocity.x += cosf(forceDirection) * forceMagnitude;
		player->velocity.y += sinf(forceDirection) * forceMagnitude;
	}
	else {
		if (2.0f * minDistance < closestPlanet->radius * closestPlanet->radius) return false;
		Vector2 planetToPlayer = Vector2Subtract(closestPlanet->position, player->position);
		float forceMagnitude = GRAVITY * closestPlanet->radius / minDistance;
		float forceDirection = atan2f(planetToPlayer.y, planetToPlayer.x);
		player->velocity.x += cosf(forceDirection) * forceMagnitude;
		player->velocity.y += sinf(forceDirection) * forceMagnitude;

		for (int i = 0; i < closestPlanet->moonCount; i++) {
			Moon* moon = &closestPlanet->moons[i];
			Vector2 moonToPlayer = Vector2Subtract(moon->position, player->position);
			minDistance = Vector2LengthSqr(moonToPlayer);
			if (0.25 * GRAVITY * minDistance < moon->radius * moon->radius) return false;
			float forceMagnitude = moon->radius / minDistance;
			float forceDirection = atan2f(moonToPlayer.y, moonToPlayer.x);
			player->velocity.x += cosf(forceDirection) * forceMagnitude;
			player->velocity.y += sinf(forceDirection) * forceMagnitude;
		}


	}
	return true;
}

void performScan(Player* player, Sun* sun) {

	Planet* closestPlanet = NULL;
	Moon* closestMoon = NULL;
	bool isMoonCloser = false;
	float minDistance = 3e38;
	for (int i = 0; i < sun->planetCount; i++) {
		Planet* planet = &(sun->planets[i]);
		float distance = Vector2DistanceSqr(planet->position, player->position);
		if (distance < minDistance) {
			minDistance = distance;
			closestPlanet = planet;
			isMoonCloser = false;
		}
		for (int i = 0; i < closestPlanet->moonCount; i++) {
			Moon* moon = &closestPlanet->moons[i];
			distance = Vector2DistanceSqr(moon->position, player->position);
			if (distance < minDistance) {
				minDistance = distance;
				closestMoon = moon;
				isMoonCloser = true;
			}
		}
	}

	if (isMoonCloser) {
		player->sensorTargetPosition = closestMoon->position;
		player->sensorTargetDistance = sqrtf(minDistance);
		if (player->sensorTargetDistance < player->sensorRange + closestMoon->radius) {
			if (closestMoon->health > 0) {
				player->sensor = 0;
				if (closestMoon->armor > 0) {
					closestMoon->armor--;
				}
				else {
					closestMoon->health--;
					player->science++;
					closestMoon->armor = closestMoon->armorMax;
				}
			}
		}
	}
	else {
		player->sensorTargetPosition = closestPlanet->position;
		player->sensorTargetDistance = sqrtf(minDistance);
		if (player->sensorTargetDistance < player->sensorRange + closestPlanet->radius) {
			if (closestPlanet->health > 0) {
				player->sensor = 0;
				if (closestPlanet->armor > 0) {
					closestPlanet->armor--;
				}
				else {
					closestPlanet->health--;
					player->science++;
					closestPlanet->armor = closestPlanet->armorMax;
				}
			}
		}
	}
}



void UpdatePlayer(Player* player, Sun* sun, const CameraState* cameraState, float* time, int playSpeed) {
	Vector2 originalPosition = player->position;
	Vector2 originalVelocity = player->velocity;
	float currentTime = *time;


	if (player->sensor >= player->sensorMax) {
		performScan(player, sun);
	}
	else {
		player->sensor += playSpeed;
	}

	for (int i = 0; i < MAX_PATH_POINTS; i++) {
		if (ApplyGravity(player, sun, currentTime)) {
			player->position = Vector2Add(player->position, player->velocity);
			player->path[i] = player->position;
			player->path[i].y *= cameraState->tilt;
			currentTime += TIME_STEP;
		}
		else {
			player->numPoints = i;
			break;
		}
		player->numPoints = MAX_PATH_POINTS;
	}


	player->position = originalPosition;
	player->velocity = originalVelocity;
	player->relativeVelocity = Vector2Distance(player->path[0], player->path[1]);

	if (playSpeed > 0) {

		if (!ApplyGravity(player, sun, *time) || player->fuel < 0) InitPlayer(player, sun, time);
		player->position = Vector2Add(player->position, player->velocity);
	}


	if ((*time - timeRecord) > 0.1f) {
		timeRecord = *time;
		player->pathHistory[player->pathHistoryCount++] = player->position;
		player->pathHistoryCountRecord = player->pathHistoryCountRecord > player->pathHistoryCount ? player->pathHistoryCountRecord : player->pathHistoryCount;
	}

}

