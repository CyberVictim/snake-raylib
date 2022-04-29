#include <stdio.h>
#include <time.h>

#include "raylib.h"

#include "main.h"

#include "apple.h"
#include "snake.h"
#include "utils_snake.h"

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
    // fixed size blocks per field
    int maxBlocks = ((int)gameField.width / snakeBlockSize) *
                    ((int)gameField.height / snakeBlockSize);
    SnakeBlock *snakeBlocks =
        MemAlloc(sizeof(SnakeBlock) * maxBlocks); // array of all blocks
    unsigned short blocksCounter = 0; // Keep tracking the next block address
    Snake snakePlayer;
    InitSnake(&snakePlayer, &gameField, snakeBlockSize, snakeBlocks);
    blocksCounter++;

    // Game loop outer vars
    float dtSnake = 0.0f;
    float dtApple = APPLE_SPEED - 1.0f; // initial time for apple is 1 second
    bool appleActive = false;
    bool isScreenFull = false;
    Rectangle apple = {0.0f, 0.0f, snakeBlockSize, snakeBlockSize};

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

        // Process snake movement
        dtSnake += GetFrameTime();
        if (dtSnake >= SNAKE_SPEED || UpdateSnakeDirection(&snakePlayer))
        {

            float Fieldbounds[4];
            GetBounds(gameField, Fieldbounds);
            UpdateSnakePosition(&snakePlayer, Fieldbounds);
            dtSnake = 0.0f;

            // Check if apple being eaten
            if (appleActive)
            {
                if (IsAppleInSnake(&apple, &snakePlayer, true))
                {
                    EatApple(&apple, &snakePlayer, &snakeBlocks[blocksCounter]);

                    if ((++blocksCounter) == maxBlocks)
                        isScreenFull = true;

                    appleActive = false;
                    dtApple = APPLE_SPEED - 1.0f;
                }
            }
        }

        // Update apple
        dtApple += GetFrameTime();
        if ((dtApple >= APPLE_SPEED && !isScreenFull) || IsKeyPressed(KEY_Q))
        {
            bool appleInSnake = true;
            while (appleInSnake)
            {
                GetApple(&apple, &gameField);
                appleInSnake = IsAppleInSnake(&apple, &snakePlayer, false);
            }
            dtApple = 0.0f;
            appleActive = true;
        }

        /* --- Draw --- */
        BeginDrawing();

        ClearBackground(DARK_MAMBA);

        DrawFPS(15, 15);

        DrawRectangleRec(gameField, SOFT_GREEN);
        DrawSnake(&snakePlayer, SOFT_RED);

        if (appleActive)
            DrawRectangleRec(apple, BANANA);

        EndDrawing();
    }

    // Free resources
    MemFree(snakeBlocks);

    CloseWindow();
    return 0;
}
