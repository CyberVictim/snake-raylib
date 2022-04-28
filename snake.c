#include "raylib.h"
#include <stdio.h>

/* Defines */

// Colors
#define DARK_MAMBA                                                             \
    (Color) { 139, 120, 143, 255 }
#define SOFT_RED                                                               \
    (Color) { 195, 89, 89, 255 }
#define SOFT_GREEN                                                             \
    (Color) { 135, 193, 155, 255 }
#define BANANA                                                                 \
    (Color) { 214, 222, 146, 255 }

// Numeric values
#define MAX_FPS 120
#define FIELD_SIZE 0.8f  // % of the screen
#define SNAKE_SIZE 0.05f // % of the screen
#define SNAKE_SPEED 0.3f // in seconds

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
void DrawSnake(Snake *snake, Color color);
void UpdateSnakePosition(Snake *snake, const float bounds[4]);
int UpdateSnakeDirection(Snake *snake);
void UpdateGameField(float screenW, float screenH, Rectangle *gameField);
int GetBounds(Rectangle rec, float buf[4]);
Controls InitControls(void);
void AddSnakeBlock(Rectangle *rec, Snake *snake);
void GetApple(int size, Rectangle *rec, const Rectangle *gameField);

// Init controls
Controls CONTROLS = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

int main(void)
{
    // Config setup
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    // Init window
    const short SCREEN_W = 800;
    const short SCREEN_H = 600;
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
    int snakeBlockSize = SNAKE_SIZE * (float)SCREEN_W;
    Rectangle body = {(gameField.x + gameField.width * 0.5f) - snakeBlockSize,
                      gameField.y + gameField.height - snakeBlockSize,
                      snakeBlockSize, snakeBlockSize};
    Rectangle body1 = body;
    Rectangle body2 = body;
    Rectangle body3 = body;
    body1.y -= body1.height;
    body2.y -= body2.height;
    body3.y -= body3.height;
    SnakeBlock snakeBody0 = {&body, NULL};
    SnakeBlock snakeBody1 = {&body1, &snakeBody0};
    SnakeBlock snakeBody2 = {&body2, &snakeBody1};
    SnakeBlock snakeBody3 = {&body3, &snakeBody2};
    Snake snakePlayer = {snakeBlockSize, &snakeBody0, &snakeBody3, UP};

    // Game loop outside vars
    float dtSnake = 0.0f;
    short isAppleActive = 0;

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

        if (dtSnake >= SNAKE_SPEED || UpdateSnakeDirection(&snakePlayer))
        {
            float bounds[4];
            GetBounds(gameField, bounds);
            UpdateSnakePosition(&snakePlayer, bounds);
            dtSnake = 0.0f;
        }

        /* --- Draw --- */
        BeginDrawing();

        ClearBackground(DARK_MAMBA);

        DrawRectangleRec(gameField, SOFT_GREEN);
        DrawSnake(&snakePlayer, SOFT_RED);

        Rectangle apple;
        if (IsKeyPressed(KEY_Q))
        {
            GetApple(snakeBlockSize, &apple, &gameField);
            isAppleActive = 1;
        }
        if (isAppleActive)
            DrawRectangleRec(apple, BANANA);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

// Function implementations
void DrawSnake(Snake *snake, Color color)
{
    // Start from tail if more than one block, otherwise just draw head
    SnakeBlock *block =
        (snake->head == snake->tail ? snake->head : snake->tail);
    while (block)
    {
        DrawRectangleRec(*block->body, color);
        block = block->next;
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

int UpdateSnakeDirection(Snake *snake)
{
    int result = 0;
    KeyboardKey pressedKey = GetKeyPressed();
    if (pressedKey)
    {
        if (pressedKey == CONTROLS.snakeDown)
        {
            if (snake->direction != UP)
                snake->direction = DOWN;
        } else if (pressedKey == CONTROLS.snakeLeft)
        {
            if (snake->direction != RIGHT)
                snake->direction = LEFT;
        } else if (pressedKey == CONTROLS.snakeRight)
        {
            if (snake->direction != LEFT)
                snake->direction = RIGHT;
        } else if (pressedKey == CONTROLS.snakeUp)
        {
            if (snake->direction != DOWN)
                snake->direction = UP;
        }
        result = 1;
    }
    return result;
}

void UpdateSnakePosition(Snake *snake, const float bounds[4])
{
    // float dt = GetFrameTime();
    switch (snake->direction)
    {
    case UP:
        if (snake->head->body->y == bounds[1])
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y = bounds[3] - snake->tail->body->height;
        } else
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y =
                snake->head->body->y - snake->head->body->height;
        }
        break;
    case DOWN:
        if ((snake->head->body->y + snake->head->body->height) == bounds[3])
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y = bounds[1];
        } else
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y =
                snake->head->body->y + snake->head->body->height;
        }
        break;
    case LEFT:
        if (snake->head->body->x == bounds[0])
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x = bounds[2] - snake->tail->body->width;
        } else
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x =
                snake->head->body->x - snake->head->body->height;
        }
        break;
    case RIGHT:
        if ((snake->head->body->x + snake->head->body->width) == bounds[2])
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x = bounds[0];
        } else
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x =
                snake->head->body->x + snake->head->body->height;
        }
        break;
    }

    // If more than one snakeBlock, put tail in head
    if (!(snake->head == snake->tail))
    {
        SnakeBlock *tailCopy = snake->tail;
        snake->tail = tailCopy->next;
        tailCopy->next = NULL;
        snake->head->next = tailCopy;
        snake->head = tailCopy;
    }
}

void AddSnakeBlock(Rectangle *rec, Snake *snake)
{
    SnakeBlock newBlock = {rec, NULL};
    snake->head->next = &newBlock;
    snake->head = &newBlock;
}

void GetApple(int size, Rectangle *rec, const Rectangle *gameField)
{
    int rangeX = gameField->width / size - 1;
    int rangeY = gameField->height / size - 1;
    rec->height = rec->width = size;
    int x = GetRandomValue(0, rangeX);
    int y = GetRandomValue(0, rangeY);
    rec->x = x * size + gameField->x;
    rec->y = y * size + gameField->y;
}

Controls InitControls(void)
{
    return (Controls){KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
}