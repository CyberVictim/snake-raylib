#include "raylib.h"
#include <stdio.h>

/* Defines */

// Colors
#define DARK_MAMBA                                                             \
    (Color) { 139, 120, 143, 1 }

// Numeric values
#define MAX_FPS (int)60
#define FIELD_SIZE (float)0.8
#define SNAKE_SIZE (float)0.2

// String literals
#define TITLE (char *)"Snake"

// Game entities
typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;

typedef struct Snake {
    float size;
    Rectangle *head;
    Rectangle *tail;
    DIRECTION direction : 2; // 0, 1, 2, 3 (four directions)
} Snake;

typedef struct Controls {
    KeyboardKey snakeUp;
    KeyboardKey snakeDown;
    KeyboardKey snakeLeft;
    KeyboardKey snakeRight;
} Controls;

// Function declarations
void DrawSnake(Snake *snake);
void UpdateSnake(Snake *snake);
Controls InitControls(void);

int main(void)
{
    // Config setup
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // Init window
    const int SCREEN_W = 1600;
    const int SCREEN_H = 900;
    InitWindow(SCREEN_W, SCREEN_H, TITLE);

    SetTargetFPS(MAX_FPS);
    SetExitKey(0);

    // Set window icon
    Image icon = LoadImage("../resources/icon.png");
    printf("----- %d\n", icon.format);
    SetWindowIcon(icon);

    // Init controls
    const Controls CONTROLS = InitControls();

    // Init game field in the center of the screen
    Rectangle gameField = {0, 0, (float)SCREEN_W * FIELD_SIZE,
                           (float)SCREEN_H * FIELD_SIZE};
    gameField.x = (SCREEN_W - gameField.width) * 0.5f;
    gameField.y = (SCREEN_H - gameField.height) * 0.5f;

    // Init snake struct
    float snakeSize = SNAKE_SIZE * gameField.width;
    Rectangle snakeBody0 = {gameField.x + gameField.width * 0.5,
                            gameField.y + gameField.height, snakeSize,
                            snakeSize};
    Snake snakePlayer = {snakeSize, &snakeBody0, NULL, UP};

    // Game loop
    while (!WindowShouldClose())
    {
        // Set exit combination
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_Q))
        {
            break;
        }

        // Update state
        if (IsWindowResized())
        {
            float screenW = (float)GetScreenWidth();
            float screenH = (float)GetScreenHeight();
            gameField = (Rectangle){0, 0, (float)screenW * FIELD_SIZE,
                                    (float)screenH * FIELD_SIZE};
            gameField.x = (screenW - gameField.width) * 0.5f;
            gameField.y = (screenH - gameField.height) * 0.5f;
        }
        // TODO: snake controls
        // DIRECTION snakeDirection;
        // switch (GetKeyPressed()) 
        // {
        // case KEY_UP:
        //     break;
        // }

        // Draw
        BeginDrawing();

        ClearBackground(DARK_MAMBA);

        DrawRectangleRec(gameField, GREEN);
        DrawSnake(&snakePlayer);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

// Function implementations
void DrawSnake(Snake *snake) { DrawRectangleRec(*snake->head, RED); }

void UpdateSnake(Snake *snake)
{
    switch (snake->direction)
    {
    case UP:
        snake->head->y -= snake->size;
        break;
    case DOWN:
        snake->head->y += snake->size;
        break;
    case LEFT:
        snake->head->x -= snake->size;
        break;
    case RIGHT:
        snake->head->x += snake->size;
        break;
    }
    return;
}

Controls InitControls(void)
{
    return (Controls){KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
}