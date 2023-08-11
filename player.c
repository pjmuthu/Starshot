#include "player.h"
#include "raymath.h"
#include "map.h"

void InitPlayer(Player* player, Sun* sun, float* time) {
	player->position = sun->startPosition;
	player->velocity = sun->startVelocity;
	player->rotation = 0;
	player->acceleration = 0.005;
	player->numPoints = MAX_PATH_POINTS;
	player->collisionObject.moonCount = -1;
	*time = 0;
}

bool ApplyGravity(Player* player, Sun* sun, float time) {


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
		if (minDistance > sun->radius * sun->radius) {
			float forceMagnitude = sun->radius / minDistance;
			float forceDirection = atan2f(sunToPlayer.y, sunToPlayer.x);
			player->velocity.x += cosf(forceDirection) * forceMagnitude;
			player->velocity.y += sinf(forceDirection) * forceMagnitude;
		}
		if (2.0f * minDistance < (sun->radius * sun->radius)) {
			return false;
		}
	}
	else
	{
		Vector2 planetToPlayer = Vector2Subtract(closestPlanet->position, player->position);
		if (minDistance > closestPlanet->radius * closestPlanet->radius) {
			float forceMagnitude = 4.0f * closestPlanet->radius / minDistance;
			float forceDirection = atan2f(planetToPlayer.y, planetToPlayer.x);
			player->velocity.x += cosf(forceDirection) * forceMagnitude;
			player->velocity.y += sinf(forceDirection) * forceMagnitude;
		}
		if (2.0f * minDistance < closestPlanet->radius * closestPlanet->radius) {
			return false;
		}

		for (int i = 0; i < closestPlanet->moonCount; i++) {
			Moon* moon = &closestPlanet->moons[i];
			Vector2 moonToPlayer = Vector2Subtract(moon->position, player->position);
			minDistance = Vector2LengthSqr(moonToPlayer);
			if (minDistance > moon->radius * moon->radius) {
				float forceMagnitude = moon->radius / minDistance;
				float forceDirection = atan2f(moonToPlayer.y, moonToPlayer.x);
				player->velocity.x += cosf(forceDirection) * forceMagnitude;
				player->velocity.y += sinf(forceDirection) * forceMagnitude;
			}
			//else if (2 * minDistance > moon->radius * moon->radius) {
			//	return false;
			//}
		}

	}
	return true;
}


void UpdatePlayer(Player* player, Sun* sun, const CameraState* cameraState, float* time) {
	Vector2 originalPosition = player->position;
	Vector2 originalVelocity = player->velocity;
	float currentTime = *time;

	for (int i = 0; i < MAX_PATH_POINTS; i++) {
		if (ApplyGravity(player, sun, currentTime)) {
			player->position = Vector2Add(player->position, player->velocity);
			player->path[i] = player->position;
			player->path[i].y *= cameraState->tilt;
			currentTime += step_time;
		}
		else {
			player->numPoints = i;
			break;
		}
		player->numPoints = MAX_PATH_POINTS;
	}

	player->position = originalPosition;
	player->velocity = originalVelocity;

	if (!ApplyGravity(player, sun, *time)) {
		InitPlayer(player, sun, time);
	}
	player->position = Vector2Add(player->position, player->velocity);
}

