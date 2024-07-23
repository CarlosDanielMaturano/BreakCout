#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 420
#define PEDAL_WIDTH 100.0
#define BALL_SPEED 5.0f

typedef struct Object {
    Vector2 pos;
    Vector2 dir;
    float w;
    float h;
    Color color;
} Object;

int main(void) {

    // player
    Object pedal = {
        .pos =  { .x = (SCREEN_WIDTH - PEDAL_WIDTH) / 2, .y = SCREEN_HEIGHT - 50.0},
        .dir = { .x = 6.5, .y = 0.0 },
        .color = RED,
        .w = PEDAL_WIDTH,
        .h = 20.0,
    };

    Object ball = {
        .pos = { 0.0, 0.0 },
        .dir = { BALL_SPEED, BALL_SPEED },
        .color = GREEN,
        .w = 20,
        .h = 20,
    };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {


        /* ==== PLAYER UPDATE ==== */
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            pedal.pos.x += pedal.dir.x;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            pedal.pos.x -= pedal.dir.x;

        // dont let the pedal go out of bounds
        if (pedal.pos.x >= SCREEN_WIDTH - pedal.w) pedal.pos.x = SCREEN_WIDTH - pedal.w;
        if (pedal.pos.x <= 0) pedal.pos.x = 0;

        /* ==== BALL UPDATE ==== */
        // normalize ball dir vector for fixing it diagonal movement, 
        // the, scale it 
        Vector2 dir = Vector2Scale(Vector2Normalize(ball.dir), 5.0);
        ball.pos  = Vector2Add(ball.pos, dir);

        if (ball.pos.x >= SCREEN_WIDTH - ball.w || ball.pos.x <= 0)
            ball.dir.x *= -1.0;

        if (ball.pos.y >= SCREEN_HEIGHT - ball.h || ball.pos.y <= 0)
            ball.dir.y *= -1.0;

        BeginDrawing();
            DrawRectangle(pedal.pos.x, pedal.pos.y, pedal.w, pedal.h, pedal.color);
            DrawRectangle(ball.pos.x, ball.pos.y, ball.w, ball.h, ball.color);
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
