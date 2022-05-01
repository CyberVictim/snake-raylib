#include <stdio.h>
#include <time.h>

#include "raylib.h"

#include "main.h"

#include "apple.h"
#include "snake.h"
#include "utils_snake.h"

#ifdef DEBUG
int logIsAppleInSnake = 0;
#endif // DEBUG

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
    printf("%d\n", gameData.GAME_STATE);
    LogCheckGameRatios(gameData.blockSize, (int)gameData.gameField.width);
#endif // DEBUG

    // Game loop
    while (!WindowShouldClose() && gameData.GAME_STATE != GAME_EXIT)
    {

        CheckExitInput(&gameData);

        switch (gameData.GAME_STATE)
        {
        case GAME_MENU:
            // init game menu here
            break;

        case GAME_OVER:
            GameOverScreenLoop(&gameData);
            break;

        case GAME_SCREEN_FILLED:
            DrawText("game is finished you did it",
                     gameData.gameField.x + gameData.gameField.width * 0.4,
                     gameData.gameField.y - 20, 20, BANANA);
            break;

        case GAME_ON:
            UpdateSnake(&gameData);
            UpdateApple(&gameData);
            break;
#ifndef DEBUG
        default:
            break;
#endif // !DEBUG
        }

        // Handle resize, unfinished
        if (IsWindowResized())
        {
            UpdateGameField((float)GetScreenWidth(), (float)GetScreenHeight(),
                            &gameData.gameField, FIELD_SIZE);
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
