#include "raylib.h"
#include "raymath.h"
#define SCREEN_WIDTH 660
#define SCREEN_HEIGHT 600
#define INITIAL_PEDAL_X (SCREEN_WIDTH - PEDAL_WIDTH) / 2 
#define INITIAL_PEDAL_Y SCREEN_HEIGHT - 50.0
#define PEDAL_WIDTH 100.0
#define PEDAL_SPEED 6.5
#define BALL_SIZE 20
#define BALL_SPEED 5.0
#define BLOCK_X_COUNT 10
#define BLOCK_Y_COUNT 7
#define BLOCKS_COUNT BLOCK_X_COUNT * BLOCK_Y_COUNT

typedef struct Object {
    Rectangle rect;
    Vector2 dir;
    Color color;
    char is_valid;
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
    .is_valid = 1,
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
    .is_valid = 1,
};

Object blocks[BLOCKS_COUNT];

Color rainbow_colors[] = {
    (Color) { 255, 0, 0, 255 }, // red
    (Color) { 255, 165, 0, 255 }, // orange
    (Color) { 255, 255, 0, 255 }, // yellow
    (Color) { 0, 128, 0, 255 }, // green
    (Color) { 0, 0, 255, 255 }, // blue
    (Color) { 75, 0, 130,255 }, // indigo
    (Color) { 238, 130, 238,255}, // violet
};

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
        .y = 80,
    };
    int idx = 0;
    for (int j = 0; j < BLOCK_Y_COUNT; j++) {
        for (int i = 0; i < BLOCK_X_COUNT; i++) {
            blocks[idx++] = (Object) {
                .rect = (Rectangle) {
                    .x = block_pos.x,
                    .y = block_pos.y,
                    .width = 50,
                    .height = 20,
                },
                .dir = (Vector2) { 0.0, 0.0 },
                .color  = rainbow_colors[j],
                .is_valid = 1,
            };
            block_pos.x += 60;
        }
        block_pos.x = 30;
        block_pos.y += 30;
    };
};

// returns true if a collide with b
int apply_vertical_collision(Object *a, Object *b) {
    if (CheckCollisionRecs(b->rect, a->rect)) {
        // if a is going down, and collide with b,
        // it's bottom y value  becomes the b's top y value
        if (a->dir.y > 0)
            a->rect.y = b->rect.y - a->rect.height;
        // if a is going up, and collide with  b,
        // it's top y value becomes the b's bottom y value
        else if (a->dir.y < 0)
            a->rect.y = b->rect.y + b->rect.height;
        a->dir.y *= -1;
        return 1;
    }
    return 0;
}

// returns true if a collide with b
int apply_horizontal_collision(Object *a, Object *b) {
    if (CheckCollisionRecs(b->rect, a->rect)) {
        // if a is going right, and collide with b,
        // it's right x value becomes the b's left x value
        if (a->dir.x > 0)
            a->rect.x = b->rect.x - a->rect.width;
        // if a is going right, and collide with  b,
        // it's left side x value becomes the b's right x value
        else if (a->dir.x < 0)
            a->rect.x = b->rect.x + b->rect.width;
        a->dir.x *= -1;
        return 1;
    }
    return 0;
}

int main(void) {
    create_blocks();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");
    SetTargetFPS(60);

    float ball_dir_scale = 5.0;

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
        ball_dir_scale += 0.001;
        // normalize ball dir vector for fixing it diagonal movement, 
        // then, scale it 
        ball.dir = Vector2Scale(Vector2Normalize(ball.dir), ball_dir_scale);
        ball.rect.y += ball.dir.y;

        if (ball.rect.y >= SCREEN_HEIGHT - ball.rect.height || ball.rect.y <= 0)
            ball.dir.y *= -1.0;
        apply_vertical_collision(&ball, &pedal);
        for (int i = 0; i < BLOCKS_COUNT; i++)  
            if (blocks[i].is_valid &&
                apply_vertical_collision(&ball, &blocks[i])) {
                blocks[i].is_valid = 0;
            }

        ball.rect.x += ball.dir.x;
        if (ball.rect.x >= SCREEN_WIDTH - ball.rect.width || ball.rect.x <= 0)
            ball.dir.x *= -1.0;
        apply_horizontal_collision(&ball, &pedal);
        for (int i = 0; i < BLOCKS_COUNT; i++) 
            if (blocks[i].is_valid &&
                apply_horizontal_collision(&ball, &blocks[i])) {
                blocks[i].is_valid = 0;
            }


        BeginDrawing();
            DrawRectangleRec(pedal.rect, pedal.color);
            DrawRectangleRec(ball.rect, ball.color);
            for (int i = 0; i < BLOCKS_COUNT; i++) {
                Object block = blocks[i];
                if (block.is_valid) 
                    DrawRectangleRec(block.rect, block.color);
            }
            ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
