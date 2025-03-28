#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_CARS 4

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
} Car;

typedef struct {
    Rectangle area;
    bool isGreen;
    float timer;
    float greenDuration;
    float redDuration;
} TrafficLight;

Car cars[MAX_CARS];
TrafficLight trafficLights[4]; // Four traffic lights at the edges

void InitializeCars() {
    // Two horizontal cars
    cars[0].position = (Vector2){ 50, 270 }; // Left side, moving right
    cars[0].velocity = (Vector2){ 2, 0 };
    
    cars[1].position = (Vector2){ SCREEN_WIDTH - 50, 310 }; // Right side, moving left
    cars[1].velocity = (Vector2){ -2, 0 };
    
    // Two vertical cars
    cars[2].position = (Vector2){ 370, 50 }; // Top side, moving down
    cars[2].velocity = (Vector2){ 0, 2 };
    
    cars[3].position = (Vector2){ 420, SCREEN_HEIGHT - 50 }; // Bottom side, moving up
    cars[3].velocity = (Vector2){ 0, -2 };
    
    for (int i = 0; i < MAX_CARS; i++) {
        cars[i].color = (Color){ rand() % 256, rand() % 256, rand() % 256, 255 };
    }
}

void InitializeTrafficLights() {
    // Traffic lights placed at the intersection's edges
    trafficLights[0] = (TrafficLight){ {200, 250, 20, 60}, true, 0.0f, 5.0f, 5.0f }; // Left side
    trafficLights[1] = (TrafficLight){ {580, 250, 20, 60}, false, 0.0f, 5.0f, 5.0f }; // Right side
    trafficLights[2] = (TrafficLight){ {370, 150, 60, 20}, false, 0.0f, 5.0f, 5.0f }; // Top side
    trafficLights[3] = (TrafficLight){ {370, 430, 60, 20}, false, 0.0f, 5.0f, 5.0f }; // Bottom side
}


void UpdateTrafficLights(float deltaTime) {
    for (int i = 0; i < 4; i++) {
        trafficLights[i].timer += deltaTime;
        if (trafficLights[i].isGreen && trafficLights[i].timer >= trafficLights[i].greenDuration) {
            trafficLights[i].isGreen = false;
            trafficLights[i].timer = 0;
        } else if (!trafficLights[i].isGreen && trafficLights[i].timer >= trafficLights[i].redDuration) {
            trafficLights[i].isGreen = true;
            trafficLights[i].timer = 0;
        }
    }
}

void UpdateCars() {
    for (int i = 0; i < MAX_CARS; i++) {
        bool canMove = false;
        if (i < 2) { // Horizontal cars
            canMove = trafficLights[i].isGreen || cars[i].position.x < trafficLights[i].area.x - 30;
        } else { // Vertical cars
            canMove = trafficLights[i].isGreen || cars[i].position.y < trafficLights[i].area.y - 30;
        }
        
        if (canMove) {
            cars[i].position.x += cars[i].velocity.x;
            cars[i].position.y += cars[i].velocity.y;
            
            // Reset car positions when off-screen
            if (cars[i].position.x > SCREEN_WIDTH) cars[i].position.x = -50;
            if (cars[i].position.x < -50) cars[i].position.x = SCREEN_WIDTH;
            if (cars[i].position.y > SCREEN_HEIGHT) cars[i].position.y = -50;
            if (cars[i].position.y < -50) cars[i].position.y = SCREEN_HEIGHT;
        }
    }
}

void DrawScene() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw Grass (Surrounding the intersection)
    DrawRectangle(0, 0, SCREEN_WIDTH, 200, GREEN);
    DrawRectangle(0, 400, SCREEN_WIDTH, 200, GREEN);
    DrawRectangle(0, 0, 300, SCREEN_HEIGHT, GREEN);
    DrawRectangle(500, 0, 300, SCREEN_HEIGHT, GREEN);

    // Draw Sidewalks
    DrawRectangle(0, 200, SCREEN_WIDTH, 20, LIGHTGRAY);
    DrawRectangle(0, 380, SCREEN_WIDTH, 20, LIGHTGRAY);
    DrawRectangle(300, 0, 20, SCREEN_HEIGHT, LIGHTGRAY);
    DrawRectangle(480, 0, 20, SCREEN_HEIGHT, LIGHTGRAY);

    // Draw Road (Intersection)
    DrawRectangle(0, 220, SCREEN_WIDTH, 160, DARKGRAY);
    DrawRectangle(320, 0, 160, SCREEN_HEIGHT, DARKGRAY);

    // Draw Lane Markings (Dotted Lines)
    for (int i = 0; i < SCREEN_WIDTH; i += 40) {
        DrawRectangle(i, 300, 20, 5, YELLOW);
    }
    for (int i = 0; i < SCREEN_HEIGHT; i += 40) {
        DrawRectangle(400, i, 5, 20, YELLOW);
    }

    // Draw Cars
    for (int i = 0; i < MAX_CARS; i++) {
        if (i < 2) {
            // Horizontal Cars (40x20)
            DrawRectangleV(cars[i].position, (Vector2){ 40, 20 }, cars[i].color);
        } else {
            // Vertical Cars (20x40) → Rotated
            DrawRectangleV(cars[i].position, (Vector2){ 20, 40 }, cars[i].color);
        }
    }

    // Draw Traffic Lights at the edges
    for (int i = 0; i < 4; i++) {
        DrawRectangleRec(trafficLights[i].area, GRAY);
        if (trafficLights[i].isGreen) {
            DrawCircle(trafficLights[i].area.x + 10, trafficLights[i].area.y + 10, 8, GREEN);
        } else {
            DrawCircle(trafficLights[i].area.x + 10, trafficLights[i].area.y + 10, 8, RED);
        }
    }

    DrawText("Traffic Simulation - Crossroad", 10, 10, 20, BLACK);
    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Traffic Simulation");
    SetTargetFPS(60);
    InitializeCars();
    InitializeTrafficLights();
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        DrawScene();
        UpdateTrafficLights(deltaTime);
        UpdateCars();
    }
    
    CloseWindow();
    return 0;
}
