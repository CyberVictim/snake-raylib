#include "raylib.h"
#include <stdio.h>

/* Defines */

// Colors
#define DARK_MAMBA                                                             \
    (Color) { 139, 120, 143, 1 }

// Numeric values
#define MAX_FPS (int)60
#define FIELD_SIZE 0.8f
#define SNAKE_SIZE 5
#define SNAKE_SPEED 0.7f

// String literals
#define TITLE (char *)"Snake"

// Game entities
typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;

typedef struct Snake {
    float size;
    float speed;
    Rectangle *head;
    Rectangle *tail;
    DIRECTION direction : 3; // 0, 1, 2, 3 (four directions)
} Snake;

typedef struct Controls {
    KeyboardKey snakeUp;
    KeyboardKey snakeDown;
    KeyboardKey snakeLeft;
    KeyboardKey snakeRight;
} Controls;

// Function declarations
void DrawSnake(Snake *snake);
void UpdateSnakePosition(Snake *snake, const float bounds[4]);
void UpdateSnakeDirection(Snake *snake);
void UpdateGameField(float screenW, float screenH, Rectangle *gameField);
int GetBounds(Rectangle rec, float buf[4]);
Controls InitControls(void);

// Init controls
Controls CONTROLS = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

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

    // Init game field in the center of the screen
    Rectangle gameField;
    UpdateGameField((float)SCREEN_W, (float)SCREEN_H, &gameField);

    // Init snake struct
    float snakeSize = SNAKE_SIZE * gameField.width * 0.01f;
    Rectangle snakeBody0 = {(gameField.x + gameField.width * 0.5f) - snakeSize,
                            gameField.y + gameField.height - snakeSize,
                            snakeSize, snakeSize};
    Snake snakePlayer = {snakeSize, SNAKE_SPEED * snakeSize, &snakeBody0, NULL,
                         UP};

    // Game loop outside vars
    float dtSnake = 0.0f;

    // Game loop
    while (!WindowShouldClose())
    {
        // Set exit combination
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_Q))
        {
            break;
        }

        /* --- Update state --- */

        // Handle resize
        if (IsWindowResized())
        {
            UpdateGameField((float)GetScreenWidth(), (float)GetScreenHeight(),
                            &gameField);
        }

        // Update snakePlayer
        dtSnake += GetFrameTime();
        UpdateSnakeDirection(&snakePlayer);
        if (dtSnake >= SNAKE_SPEED)
        {
            float bounds[4];
            GetBounds(gameField, bounds);
            UpdateSnakePosition(&snakePlayer, bounds);
            dtSnake = 0.0f;
        }

        /* --- Draw --- */
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

// Relative to the screen
void UpdateGameField(float screenW, float screenH, Rectangle *gameField)
{
    gameField->width = (float)screenW * FIELD_SIZE;
    gameField->height = (float)screenH * FIELD_SIZE;
    gameField->x = (screenW - gameField->width) * 0.5f;
    gameField->y = (screenH - gameField->height) * 0.5f;
}

// Fill the array with bounds of the rectangle, starting from left side going
// clockwise
int GetBounds(Rectangle rec, float buf[4])
{
    buf[0] = rec.x;
    buf[1] = rec.y;
    buf[2] = rec.x + rec.width;
    buf[3] = rec.y + rec.height;
    return 0;
}

void UpdateSnakeDirection(Snake *snake)
{
    KeyboardKey pressedKey = GetKeyPressed();
    if (pressedKey)
    {
        if (pressedKey == CONTROLS.snakeDown)
        {
            snake->direction = DOWN;
        } else if (pressedKey == CONTROLS.snakeLeft)
        {
            snake->direction = LEFT;
        } else if (pressedKey == CONTROLS.snakeRight)
        {
            snake->direction = RIGHT;
        } else if (pressedKey == CONTROLS.snakeUp)
        {
            snake->direction = UP;
        }
    }
}

void UpdateSnakePosition(Snake *snake, const float bounds[4])
{
    // float dt = GetFrameTime();
    switch (snake->direction)
    {
    case UP:
        snake->head->y -= snake->size;
        if (snake->head->y < bounds[1])
            snake->head->y = bounds[3] - snake->head->height;
        break;
    case DOWN:
        snake->head->y += snake->size;
        if ((snake->head->y + snake->head->height) > bounds[3])
            snake->head->y = bounds[1];
        break;
    case LEFT:
        snake->head->x -= snake->size;
        if (snake->head->x < bounds[0])
            snake->head->x = bounds[2] - snake->head->width;
        break;
    case RIGHT:
        snake->head->x += snake->size;
        if ((snake->head->x + snake->head->width) > bounds[2])
            snake->head->x = bounds[0];
        break;
    }
    return;
}

Controls InitControls(void)
{
    return (Controls){KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
}