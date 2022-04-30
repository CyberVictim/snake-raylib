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

#define DEBUG // Include debug code

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

    // GameData
    GameData gameData;
    InitGameData(&gameData, SCREEN_W, SCREEN_H);

#ifdef DEBUG
    LogCheckGameRatios(gameData.snakePlayer.blockSize,
                       (int)gameData.gameField.width);
#endif // DEBUG

    // Game loop
    while (!WindowShouldClose())
    {
        // Set exit combination
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_Q))
        {
            break;
        }

        switch (gameData.GAME_STATE)
        {
        case GAME_MENU:
            // init game menu here
            break;
        case GAME_OVER:
            DrawText("game is over",
                     gameData.gameField.x + gameData.gameField.width * 0.4,
                     gameData.gameField.y - 20, 20, BANANA);
            break;
        case GAME_SCREEN_FILLED:
            DrawText("game is finished you did it",
                     gameData.gameField.x + gameData.gameField.width * 0.4,
                     gameData.gameField.y - 20, 20, BANANA);
            break;
        case GAME_ON:
            UpdateSnake(&gameData);
            break;
        }

        /* --- Update state --- */

        // Handle resize
        if (IsWindowResized())
        {
            UpdateGameField((float)GetScreenWidth(), (float)GetScreenHeight(),
                            &gameData.gameField, FIELD_SIZE);
        }

        // Update apple
        gameData.dtApple += GetFrameTime();
        if ((gameData.dtApple >= APPLE_SPEED &&
             gameData.GAME_STATE != GAME_SCREEN_FILLED) ||
            IsKeyPressed(KEY_Q))
        {
            gameData.dtApple = APPLE_SPEED - 0.1f;
            bool appleInSnake = true;
            gameData.appleActive = false;
            // Limit the calculation by max N tries per cycle
            for (int i = 0; i < GET_APPLE_MAX_TRIES; i++)
            {
                GetApple(&gameData.apple, &gameData.gameField);
                appleInSnake =
                    IsRecInSnake(&gameData.apple, &gameData.snakePlayer);
#ifdef DEBUG
                logIsAppleInSnake++;
#endif // DEBUG
                if (!appleInSnake)
                {
                    gameData.dtApple = 0.0f;
                    gameData.appleActive = true;
                    break;
                }
            }

#ifdef DEBUG
            printf("IsAppleInSnake calls - %d\n", logIsAppleInSnake);
            logIsAppleInSnake = 0;
#endif // DEBUG
        }

        /* --- Draw --- */
        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);

        if (gameData.gameSettingsFlags & SET_SNAKE_SHOW_FPS)
        {
            DrawFPS(15, 15);
        }

        DrawRectangleRec(gameData.gameField, SOFT_GREEN);
        DrawSnake(&gameData.snakePlayer, SNAKE_BODY_COLOR, SNAKE_HEAD_COLOR);

        if (gameData.appleActive)
            DrawRectangleRec(gameData.apple, BANANA);

        EndDrawing();
    }
    // Free resources
    MemFree(gameData.snakeBlocks);

    CloseWindow();
    return 0;
}
