#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

typedef struct {
    Camera2D camera;
    Vector2 velocity;  // Add velocity vector for camera movement
    float zoomVelocity;  // Add velocity for camera zooming
    float acceleration;  // Adjust the acceleration factor as needed
    float skew;
} CameraState;

extern CameraState InitCameraState(int screenWidth, int screenHeight);
extern void UpdateCameraState(CameraState* cameraState);
extern void CloseCameraState(CameraState* cameraState);

#endif // CAMERA_H
