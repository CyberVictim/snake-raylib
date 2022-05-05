#include <stdio.h>
#include <time.h>

#include "apple.h"
#include "main.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

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

    // Init menu struct
    GameMenu gameMenu;
    InitGameMenu(&gameMenu, SCREEN_W, SCREEN_H);

    // GameData
    GameData gameData;
    InitGameData(&gameData, SCREEN_W, SCREEN_H);

#ifdef DEBUG
    printf("game state number %d\n", gameData.GAME_STATE);
    LogCheckGameRatios(gameData.blockSize, (int)gameData.gameField.width);
#endif // DEBUG

    // Game loop
    while (!WindowShouldClose() && gameData.GAME_STATE != GAME_EXIT)
    {
        CheckExitInput(&gameData);
        switch (gameData.GAME_STATE)
        {
        case GAME_MENU:
            UpdateDrawMenu(&gameMenu, &gameData);
            break;

        case GAME_OVER:
            GameStateAlert(&gameData,
                           "GAME OVER! Press Enter to restart, Escape for menu",
                           SCREEN_H);
            break;

        case GAME_SCREEN_FILLED:
            GameStateAlert(
                &gameData,
                "SNAKE IS FULL! Press Enter to restart, Escape for menu",
                SCREEN_H);
            break;

        case GAME_ON:
            UpdateSnake(&gameData);
            UpdateApple(&gameData);
            DrawGame(&gameData);
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
    }
    // Free resources
    MemFree(gameData.snakeBlocks);
    MemFree(gameMenu.buttonGroup.name);
    MemFree(gameMenu.bPlay.name);
    MemFree(gameMenu.bExit.name);

    CloseWindow();
    return 0;
}
