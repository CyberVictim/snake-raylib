#include <stdio.h>
#include <time.h>

#include "raylib.h"

#include "main.h"

#include "apple.h"
#include "snake.h"
#include "utils.h"

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
    SnakeBlock snakeBody0 = {&body, NULL};
    Snake snakePlayer = {snakeBlockSize, &snakeBody0, &snakeBody0, UP};

    // Game loop outer vars
    float dtSnake = 0.0f;
    float dtApple = APPLE_SPEED - 1.0f;
    Rectangle *apple = NULL;

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

        // Update apple
        dtApple += GetFrameTime();
        if (dtApple >= APPLE_SPEED || IsKeyPressed(KEY_Q))
        {
            GetApple(snakeBlockSize, &apple, &gameField);
            dtApple = 0.0f;
        }

        /* --- Draw --- */
        BeginDrawing();

        ClearBackground(DARK_MAMBA);

        DrawFPS(15, 15);

        DrawRectangleRec(gameField, SOFT_GREEN);
        DrawSnake(&snakePlayer, SOFT_RED);

        if (apple)
            DrawRectangleRec(*apple, BANANA);

        EndDrawing();
    }
    MemFree(apple);
    CloseWindow();
    return 0;
}
