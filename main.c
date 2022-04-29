#include <stdio.h>
#include <time.h>

#include "raylib.h"

#include "main.h"

#include "apple.h"
#include "snake.h"
#include "utils_snake.h"

// Init controls
Controls CONTROLS = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

// DELETE! temp dev vars
int logIsAppleInSnake = 0;

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
    float dtApple = APPLE_SPEED - APPLE_SPEED_FIRST; // initial time for apple
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
        if (UpdateSnakeDirection(&snakePlayer) || dtSnake >= SNAKE_SPEED)
        {

            dtSnake = 0.0f;
            float Fieldbounds[4];
            GetBounds(gameField, Fieldbounds);

            // Update position and process eating apple if function returns 1
            if (UpdateSnakePosition(&snakePlayer, Fieldbounds,
                                    appleActive ? &apple : NULL))
            {
                EatApple(&apple, &snakePlayer, &snakeBlocks[blocksCounter]);

                if ((++blocksCounter) == maxBlocks)
                    isScreenFull = true; // Can this happen?

                appleActive = false;
                dtApple = APPLE_SPEED - APPLE_SPEED_FIRST;
            }
        }

        // Update apple
        dtApple += GetFrameTime();
        if ((dtApple >= APPLE_SPEED && !isScreenFull) || IsKeyPressed(KEY_Q))
        {
            dtApple = APPLE_SPEED - 0.1f;
            bool appleInSnake = true;
            appleActive = false;
            // Limit the calculation by max N tries per cycle
            for (int i = 0; i < GET_APPLE_MAX_TRIES; i++)
            {
                GetApple(&apple, &gameField);
                appleInSnake = IsAppleInSnake(&apple, &snakePlayer, false);
                logIsAppleInSnake++;
                if (!appleInSnake)
                {
                    dtApple = 0.0f;
                    appleActive = true;
                    break;
                }
            }

            // temp log
            printf("IsAppleInSnake calls - %d\n", logIsAppleInSnake);
            logIsAppleInSnake = 0;
        }

        /* --- Draw --- */
        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);

        DrawFPS(15, 15);

        DrawRectangleRec(gameField, SOFT_GREEN);
        DrawSnake(&snakePlayer, SNAKE_BODY_COLOR, SNAKE_HEAD_COLOR);

        if (appleActive)
            DrawRectangleRec(apple, BANANA);

        EndDrawing();
    }
    // Free resources
    MemFree(snakeBlocks);

    CloseWindow();
    return 0;
}
