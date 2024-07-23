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
        // then, scale it 
        ball.dir = Vector2Scale(Vector2Normalize(ball.dir), 5.0);

        ball.rect.y += ball.dir.y;
        if (ball.rect.y >= SCREEN_HEIGHT - ball.rect.height || ball.rect.y <= 0)
            ball.dir.y *= -1.0;
        if (CheckCollisionRecs(pedal.rect, ball.rect)) {
            // if the ball is going down, and collide with the pedal,
            // it's bottom y value  becomes the pedal's top y value
            if (ball.dir.y > 0)
                ball.rect.y = pedal.rect.y - ball.rect.height;
            // if the ball is going up, and collide with the pedal,
            // it's top y value becomes the pedal's bottom y value
            else if (ball.dir.y < 0)
                ball.rect.y = pedal.rect.y + pedal.rect.height;
            ball.dir.y *= -1;
        }

        ball.rect.x += ball.dir.x;
        if (ball.rect.x >= SCREEN_WIDTH - ball.rect.width || ball.rect.x <= 0)
            ball.dir.x *= -1.0;
        if (CheckCollisionRecs(pedal.rect, ball.rect)) {
            // if the ball is going right, and collide with the pedal,
            // it's right x value becomes the pedal's left x value
            if (ball.dir.x > 0)
                ball.rect.x = pedal.rect.x - ball.rect.width;
            // if the ball is going right, and collide with the pedal,
            // it's left side x value becomes the pedal's right x value
            else if (ball.dir.x < 0)
                ball.rect.x = pedal.rect.x + pedal.rect.width;
            ball.dir.x *= -1;
        }

        BeginDrawing();
            DrawRectangleRec(pedal.rect, pedal.color);
            DrawRectangleRec(ball.rect, ball.color);
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
