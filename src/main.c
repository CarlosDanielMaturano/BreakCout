#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 420
#define PEDAL_WIDTH 100.0

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
        .pos =  { .x = (SCREEN_WIDTH - PEDAL_WIDTH) / 2, .y = SCREEN_HEIGHT - 50.0},
        .speed = 6.5,
        .color = RED,
        .w = PEDAL_WIDTH,
        .h = 20.0,
    };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            pedal.pos.x += pedal.speed;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            pedal.pos.x -= pedal.speed;

        // dont let the pedal go out of bounds
        if (pedal.pos.x >= SCREEN_WIDTH - pedal.w) pedal.pos.x = SCREEN_WIDTH - pedal.w;
        if (pedal.pos.x <= 0) pedal.pos.x = 0;

        BeginDrawing();
            DrawRectangle(pedal.pos.x, pedal.pos.y, pedal.w, pedal.h, pedal.color);
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
