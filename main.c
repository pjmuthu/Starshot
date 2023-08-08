#include "raylib.h"
#include "raymath.h"

#define FUTURE_POINTS 4096

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float acceleration;
    float rotation;
    float size;
} Spaceship;

typedef struct {
    Vector2 position;
    float radius;
    float mass;
    float angle;
    float period;
    float distance;
} Planet;

void UpdateSpaceship(Spaceship* ship, const Planet* planet)
{
    Vector2 direction = Vector2Subtract(planet->position, ship->position);
    float distance = Vector2Length(direction);
    float gravitationalForce = (planet->mass / (distance * distance));
    Vector2 gravity = Vector2Scale(Vector2Normalize(direction), gravitationalForce);
    ship->velocity = Vector2Add(ship->velocity, gravity);
    ship->position = Vector2Add(ship->position, ship->velocity);
}

Vector2 GetPlanetPosition(Planet* planet, int tickCount) {
    planet->angle = (tickCount / planet->period) * (2 * PI);
    return (Vector2) { planet->distance* cosf(planet->angle), planet->distance* sinf(planet->angle) };
}

void UpdatePlanet(Planet* planet, int tickCount)
{
    planet->position = GetPlanetPosition(planet, tickCount);
}



void DrawSpaceship(Spaceship ship, const Planet* planet, int tickCount)
{
    Vector2 futurePosition = ship.position;
    Vector2 futureVelocity = ship.velocity;
    Vector2 futurePositions[FUTURE_POINTS];
    bool isCollision = false;
    int pathLength = 0;
    for (int i = 0; i < FUTURE_POINTS; i++)
    {
        Vector2 futurePlanetPosition = GetPlanetPosition(planet, tickCount + i);
        Vector2 direction = Vector2Subtract(futurePlanetPosition, futurePosition);
        float distance = Vector2Length(direction);
        float gravitationalForce = (planet->mass / (distance * distance));
        Vector2 gravity = Vector2Scale(Vector2Normalize(direction), gravitationalForce);
        futureVelocity = Vector2Add(futureVelocity, gravity);
        futurePosition = Vector2Add(futurePosition, futureVelocity);
        futurePositions[i] = futurePosition;
        if (isCollision == false) {
            isCollision = CheckCollisionPointCircle(futurePosition, futurePlanetPosition, planet->radius);
            pathLength = i;
        }
        else {
            DrawCircleV(futurePlanetPosition, planet->radius, RED);
            break;
        }
    }
    for (int i = 0; i < pathLength - 1; i++)
    {
        float value = (float)i / pathLength;
        DrawLineEx(futurePositions[i], futurePositions[i + 1], 4.0f * (0.5f * (sinf(PI * value) + 1.0f)), isCollision ? RED : LIGHTGRAY);
    }
}






void DrawCircleLinesEx(int centerX, int centerY, float radius, int segments, Color color) {
    if (segments < 3) segments = 3; 
    float angle = 0;
    float angleIncrement = 360.0f / segments;
    Vector2 prevPos;
    Vector2 currPos;
    for (int i = 0; i < segments; i++) {
        currPos.x = centerX + radius * cosf(DEG2RAD * angle);
        currPos.y = centerY + radius * sinf(DEG2RAD * angle);
        if (i > 0) {
            DrawLineEx(prevPos, currPos, 1.0f, color);
        }
        prevPos = currPos;
        angle += angleIncrement;
    }
    DrawLineEx(prevPos, (Vector2) { centerX + radius * cosf(DEG2RAD * 0), centerY + radius * sinf(DEG2RAD * 0) }, 1.0f, color);
}

void DrawPlanet(Planet planet)
{
    DrawCircleV(planet.position, planet.radius, LIGHTGRAY);
    DrawCircleLinesEx(0, 0, planet.distance, 128, LIGHTGRAY);
}

int main()
{
    int screenWidth = 1280;
    int screenHeight = 720;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Spaceship Movement");

    Spaceship ship;
    ship.position = (Vector2){ 500, 500 };
    ship.velocity = (Vector2){ 0.05f, -0.1f };
    ship.acceleration = 0.0001f;
    ship.rotation = 0.0f;
    ship.size = 16.0f;

    Planet planet;
    planet.position = (Vector2){ 0, 0 };
    planet.mass = 32;
    planet.radius = 32;
    planet.period = 32 * 8196.0f;
    planet.distance = 1024.0f;

    int tickCount = 0;

    SetTargetFPS(60);
    Camera2D camera = { 0 };
    camera.target = ship.position;
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bool drawSpaceship = true;

    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            Vector2 mousePositionOffset = Vector2Subtract(mousePosition, camera.offset);
            Vector2 adjustedMousePosition = Vector2Add(mousePositionOffset, camera.target);
            ship.rotation = Vector2Angle(ship.position, adjustedMousePosition);
            ship.velocity.x += ship.acceleration * cosf(ship.rotation);
            ship.velocity.y += ship.acceleration * sinf(ship.rotation);
        }

        if (IsKeyPressed(KEY_SPACE))
            drawSpaceship = !drawSpaceship;

        if (IsWindowResized())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
            camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
        }

        UpdateSpaceship(&ship, &planet);
        UpdatePlanet(&planet, tickCount);

        const float cameraSpeed = 2.0f;
        const float cameraZoomSpeed = 0.01f;
        if (IsKeyDown(KEY_W))
            camera.target.y -= cameraSpeed;
        if (IsKeyDown(KEY_S))
            camera.target.y += cameraSpeed;
        if (IsKeyDown(KEY_A))
            camera.target.x -= cameraSpeed;
        if (IsKeyDown(KEY_D))
            camera.target.x += cameraSpeed;
        float previousZoom = camera.zoom;
        if (IsKeyDown(KEY_Z))
            camera.zoom *= 1.0f + cameraZoomSpeed;
        if (IsKeyDown(KEY_C))
            camera.zoom *= 1.0f - cameraZoomSpeed;
        Vector2 zoomOffset = {
            (screenWidth / 2) - (screenWidth / (2 * camera.zoom)),
            (screenHeight / 2) - (screenHeight / (2 * camera.zoom))
        };
        camera.offset = Vector2Add(camera.offset, Vector2Scale(zoomOffset, previousZoom - camera.zoom));

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(RAYWHITE);
        DrawCircle(0, 0, 128, LIGHTGRAY);
        DrawPlanet(planet);
        if (drawSpaceship)
            DrawSpaceship(ship, &planet, tickCount);
        DrawPolyLinesEx(ship.position, 3, ship.size, 0, 8.0f, DARKGRAY);
        EndMode2D();
        DrawFPS(10, 10);
        EndDrawing();
        tickCount++;
    }
    CloseWindow();
    return 0;
}
