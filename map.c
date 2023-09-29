#include "map.h"
#include <raymath.h>


void UpdatePlanetPosition(Planet* planet, float t) {
    float angle = fmod(planet->angle + (t / planet->period), 2 * PI);
    planet->position.x = cosf(angle) * planet->distance;
    planet->position.y = sinf(angle) * planet->distance;
}


void UpdateMoonPosition(Moon* moon, float t) {
    float angle = fmod(moon->angle + (t / moon->period), 2 * PI);
    moon->position.x = cosf(angle) * moon->distance;
    moon->position.y = sinf(angle) * moon->distance;
}

void UpdateMapPositions(Sun* sun, float t) {
    for (int i = 0; i < sun->planetCount; i++) {
        Planet* planet = &(sun->planets[i]);
        UpdatePlanetPosition(planet, t);
        for (int j = 0; j < planet->moonCount; j++) {
            Moon* moon = &(planet->moons[j]);
            UpdateMoonPosition(moon, t);
            moon->position = Vector2Add(moon->position, planet->position);
        }
    }
}

Planet* GetClosestPlanet(Vector2 position, Sun* sun, float t) {
    float minDistance = 1e37;
    Planet* closestPlanet = &(sun->planets[0]);
    for (int i = 0; i < sun->planetCount; i++) {
        Planet* planet = &(sun->planets[i]);
        UpdatePlanetPosition(planet, t);
        float distance = Vector2DistanceSqr(planet->position, position);
        if (distance < minDistance) {
            minDistance = distance;
            closestPlanet = planet;
        }
    }
    return closestPlanet;
}

void InitMap(Sun* sun) {
    for (int i = 0; i < sun->planetCount; i++) {
        sun->planets[i].armor = sun->planets[i].armorMax;
        sun->planets[i].health = sun->planets[i].healthMax;
        for (int j = 0; j < sun->planets[i].moonCount; j++) {
            sun->planets[i].moons[j].armor = sun->planets[i].moons[j].armorMax;
            sun->planets[i].moons[j].health = sun->planets[i].moons[j].healthMax;
        }
    }
}

