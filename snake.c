#include "raylib.h"
#include <stdio.h>

/* Defines */

// Colors
#define DARK_MAMBA                                                             \
    (Color) { 139, 120, 143, 1 }

// Numeric values
#define MAX_FPS (int)60
#define FIELD_SIZE 0.8f
#define SNAKE_SIZE 0.05f
#define SNAKE_SPEED 0.7f

// String literals
#define TITLE (char *)"Snake"

// Game entities
typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;

typedef struct SnakeBlock {
    Rectangle *body;
    struct SnakeBlock *next;
} SnakeBlock;

typedef struct Snake {
    float blockSize;
    SnakeBlock *head;
    SnakeBlock *tail;
    DIRECTION direction : 3; // 0, 1, 2, 3 (four directions)
    // SnakeBlock blocks[];
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
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    // Init window
    const short SCREEN_W = 1600;
    const short SCREEN_H = 900;
    InitWindow(SCREEN_W, SCREEN_H, TITLE);

    SetTargetFPS(MAX_FPS);
    SetExitKey(0); // Remove exit key (esc by default)

    // Set window icon
    Image icon = LoadImage("resources/icon.png");
    printf("----- %d\n", icon.format);
    SetWindowIcon(icon);

    // Init game field in the center of the screen
    Rectangle gameField;
    UpdateGameField((float)SCREEN_W, (float)SCREEN_H, &gameField);

    // Init snake struct
    float snakeBlockSize = SNAKE_SIZE * (float)SCREEN_W;
    Rectangle body = {(gameField.x + gameField.width * 0.5f) - snakeBlockSize,
                      gameField.y + gameField.height - snakeBlockSize,
                      snakeBlockSize, snakeBlockSize};
    SnakeBlock snakeBody0 = {&body, NULL};
    Snake snakePlayer = {snakeBlockSize, &snakeBody0, &snakeBody0, UP};

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
void DrawSnake(Snake *snake)
{
    SnakeBlock *block = snake->tail;
    while (1)
    {
        DrawRectangleRec(*block->body, RED);
        block = block->next;
        if (block == NULL)
            break;
    }
}

// Relative to the screen
void UpdateGameField(float screenW, float screenH, Rectangle *gameField)
{
    gameField->width = screenW * FIELD_SIZE;
    gameField->height = screenH * FIELD_SIZE;
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
        if (snake->head == snake->tail) // Only one snakeBlock
        {
            if (snake->head->body->y == bounds[1])
                snake->head->body->y = bounds[3] - snake->head->body->height;
            else
                snake->head->body->y -= snake->head->body->height;
        }
        break;
    case DOWN:
        if (snake->head == snake->tail)
        {
            if ((snake->head->body->y + snake->head->body->height) == bounds[3])
                snake->head->body->y = bounds[1];
            else
                snake->head->body->y += snake->head->body->height;
        }
        break;
    case LEFT:
        if (snake->head == snake->tail)
        {
            if (snake->head->body->x == bounds[0])
                snake->head->body->x = bounds[2] - snake->head->body->width;
            else
                snake->head->body->x -= snake->head->body->height;
        }
        break;
    case RIGHT:
        if (snake->head == snake->tail)
        {
            if ((snake->head->body->x + snake->head->body->width) == bounds[2])
                snake->head->body->x = bounds[0];
            else
                snake->head->body->x += snake->head->body->height;
        }
        break;
    }
    return;
}

Controls InitControls(void)
{
    return (Controls){KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
}