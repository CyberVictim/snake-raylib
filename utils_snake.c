#include "utils_snake.h"
#include "main.h"
#include "raylib.h"
#include "snake.h"
#include <stdio.h>
#include <stdlib.h>

// Relative to the screen
void UpdateGameField(float screenW, float screenH, Rectangle *gameField,
                     float field_size)
{
    gameField->width = screenW * field_size;
    gameField->height = screenH * field_size;
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

void LogCheckGameRatios(int snakeSize, int gameFieldWidth)
{
    if (gameFieldWidth % snakeSize != 0)
    {
        TraceLog(LOG_WARNING, "Snake size to gamefield ratio is wrong, asjust "
                              "SNAKE_SIZE or FIELD_SIZE");
        exit(EXIT_FAILURE);
    }
    TraceLog(LOG_INFO, "Snake size seems good");
}

void InitGameData(GameData *gameData, float SCREEN_W, float SCREEN_H)
{
    // Init game field in the center of the screen
    UpdateGameField(SCREEN_W, SCREEN_H, &gameData->gameField, FIELD_SIZE);

    // fixed size blocks per field
    int snakeBlockSize = SNAKE_SIZE * (float)gameData->gameField.width;
    int maxBlocks = ((int)gameData->gameField.width / snakeBlockSize) *
                    ((int)gameData->gameField.height / snakeBlockSize);

    // Init array of all snake blocks
    gameData->snakeBlocks = MemAlloc(sizeof(SnakeBlock) * maxBlocks);
    gameData->blocksCounter = 0; // keep track of next block's address

    // Init snakePlayer
    InitSnake(&gameData->snakePlayer, &gameData->gameField, snakeBlockSize,
              &gameData->snakeBlocks[gameData->blocksCounter++]);

    gameData->GAME_STATE = INITIAL_GAME_STATE;
    gameData->dtSnake = 0.0f;
    gameData->dtApple = APPLE_SPEED - APPLE_SPEED_FIRST; // initial  apple time
    gameData->appleActive = false;
    gameData->apple = (Rectangle){0.0f, 0.0f, snakeBlockSize, snakeBlockSize};

    // init game settings flags
    unsigned int defaultGameSettings = SET_SNAKE_SHOW_FPS;
    gameData->gameSettingsFlags = defaultGameSettings;
}
