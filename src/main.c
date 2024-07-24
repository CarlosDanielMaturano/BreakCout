#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdio.h>
#define SCREEN_WIDTH 660
#define SCREEN_HEIGHT 420
#define INITIAL_PEDAL_X (SCREEN_WIDTH - PEDAL_WIDTH) / 2 
#define INITIAL_PEDAL_Y SCREEN_HEIGHT - 50.0
#define PEDAL_WIDTH 100.0
#define PEDAL_SPEED 6.5
#define BALL_SIZE 20
#define BALL_SPEED 5.0
#define BLOCK_X_COUNT 10
#define BLOCK_Y_COUNT 5
#define BLOCKS_COUNT BLOCK_X_COUNT * BLOCK_Y_COUNT

typedef struct Object {
    Rectangle rect;
    Vector2 dir;
    Color color;
} Object;

// player's pedal 
Object pedal = {
    .rect = (Rectangle) { 
        .x = INITIAL_PEDAL_X,
        .y = INITIAL_PEDAL_Y,
        .width = PEDAL_WIDTH,
        .height = 20.0
    },
    .dir = { .x = PEDAL_SPEED, .y = 0.0 },
    .color = RED,
};

Object ball = {
    .rect = (Rectangle) { 
        .x = INITIAL_PEDAL_X + (PEDAL_WIDTH - BALL_SIZE) / 2,
        .y = INITIAL_PEDAL_Y - 2 * BALL_SIZE,
        .width = BALL_SIZE,
        .height = BALL_SIZE
    },
    .dir = { BALL_SPEED, BALL_SPEED },
    .color = GREEN,
};

Object blocks[BLOCKS_COUNT];

void reset_game() {
    pedal.rect = (Rectangle) {
        .x = INITIAL_PEDAL_X,
        .y = INITIAL_PEDAL_Y,
        .width = PEDAL_WIDTH,
        .height = 20.0,
    };
    pedal.dir = (Vector2) { .x = PEDAL_SPEED, .y = 0.0 };
    ball.rect = (Rectangle) {
        .x = INITIAL_PEDAL_X + (PEDAL_WIDTH - BALL_SIZE) / 2,
        .y = INITIAL_PEDAL_Y - 2 * BALL_SIZE,
        .width = BALL_SIZE,
        .height = BALL_SIZE
    };
    ball.dir = (Vector2) { BALL_SPEED, BALL_SPEED };
}

void create_blocks() {
    Vector2 block_pos = (Vector2) {
        .x = 30,
        .y = 10,
    };
    int idx = 0;
    for (int j = 1; j <= BLOCK_Y_COUNT; j++) {
        for (int i = 0; i < BLOCK_X_COUNT; i++) {
            blocks[idx++] = (Object) {
                .rect = (Rectangle) {
                    .x = block_pos.x,
                    .y = block_pos.y,
                    .width = 50,
                    .height = 20,
                },
                .dir = (Vector2) { 0.0, 0.0 },
                .color = RED,
            };
            block_pos.x += 60;
        }
        block_pos.x = 30;
        block_pos.y += 30;
    };
};

int main(void) {
    create_blocks();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        /* ==== PLAYER UPDATE ==== */
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            pedal.rect.x += pedal.dir.x;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            pedal.rect.x -= pedal.dir.x;

        if (IsKeyDown(KEY_W)) reset_game();

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
    
            for (int i = 0; i < BLOCKS_COUNT; i++) {
                Object block = blocks[i];
                if (block.rect.width && block.rect.height) 
                    DrawRectangleRec(block.rect, block.color);
            }

            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
