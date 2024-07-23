#include "raylib.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 420

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello, World!", SCREEN_WIDTH / 2.5, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    return 0;
}
