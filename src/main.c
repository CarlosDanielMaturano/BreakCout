#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 420

typedef struct Object {
    Vector2 pos;
    float speed;
    float w;
    float h;
    Color color;
} Object;

int main(void) {

    // player
    Object pedal = {
        .pos =  { .x = 50.0, .y = SCREEN_HEIGHT - 50.0},
        .speed = 5.0,
        .color = RED,
        .w = 100.0,
        .h = 20.0,
    };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
            DrawRectangle(pedal.pos.x, pedal.pos.y, pedal.w, pedal.h, pedal.color);
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
