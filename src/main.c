#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 420
#define PEDAL_WIDTH 100.0
#define BALL_SPEED 5.0f

typedef struct Object {
    Rectangle rect;
    Vector2 dir;
    Color color;
} Object;

int main(void) {

    // player
    Object pedal = {
        .rect = (Rectangle) { 
            .x = (SCREEN_WIDTH - PEDAL_WIDTH) / 2,
            .y = SCREEN_HEIGHT - 50.0,
            .width = PEDAL_WIDTH,
            .height = 20.0
        },
        .dir = { .x = 6.5, .y = 0.0 },
        .color = RED,
    };

    Object ball = {
        .rect = (Rectangle) { 
            .x = 0.0,
            .y = 0.0,
            .width = 20,
            .height = 20
        },
        .dir = { BALL_SPEED, BALL_SPEED },
        .color = GREEN,
    };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        /* ==== PLAYER UPDATE ==== */
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            pedal.rect.x += pedal.dir.x;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            pedal.rect.x -= pedal.dir.x;

        // dont let the pedal go out of bounds
        if (pedal.rect.x >= SCREEN_WIDTH - pedal.rect.width) 
            pedal.rect.x = SCREEN_WIDTH - pedal.rect.width;
        if (pedal.rect.x <= 0) 
            pedal.rect.x = 0;

        /* ==== BALL UPDATE ==== */
        // normalize ball dir vector for fixing it diagonal movement, 
        // the, scale it 
        Vector2 dir = Vector2Scale(Vector2Normalize(ball.dir), 5.0);
        Vector2 new_ball_pos  = Vector2Add((Vector2){ ball.rect.x, ball.rect.y}, dir);
        ball.rect.x = new_ball_pos.x;
        ball.rect.y = new_ball_pos.y;

        if (ball.rect.x >= SCREEN_WIDTH - ball.rect.width || ball.rect.x <= 0)
            ball.dir.x *= -1.0;

        if (ball.rect.y >= SCREEN_HEIGHT - ball.rect.height || ball.rect.y <= 0)
            ball.dir.y *= -1.0;

        BeginDrawing();
            DrawRectangleRec(pedal.rect, pedal.color);
            DrawRectangleRec(ball.rect, ball.color);
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
