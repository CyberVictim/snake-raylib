#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "apple.h"
#include "main.h"
#include "raylib.h"
#include "snake.h"

static bool GetApple(Rectangle *apple, const Rectangle *gameField,
                     bool **matrix)
{
    bool result = false;
    // Get range for possible fixed coordinates (0 to MAX)
    float appleSize = apple->height;
    int rangeX = gameField->width / appleSize - 1;
    int rangeY = gameField->height / appleSize - 1;

    // Get pseudo random fixed coordinate from range
    SetRandomSeed(time(NULL));
    int x = GetRandomValue(0, rangeX);
    int y = GetRandomValue(0, rangeY);

    // check if apple collides with snake
    if (matrix[y][x])
    {
        // Calculate actual screen coordinate
        apple->x = x * appleSize + gameField->x;
        apple->y = y * appleSize + gameField->y;
        result = true;
    }

    return result;
}

void UpdateApple(GameData *gameData)
{
    gameData->dtApple += GetFrameTime();

    // if enough time has passed
    if ((gameData->dtApple >= APPLE_SPEED &&
         gameData->GAME_STATE != GAME_SCREEN_FILLED))
    {
        // pause time if we don't find apple in this frame
        gameData->dtApple = APPLE_SPEED - 0.1f;
        bool isAppleValid = false;
        gameData->appleActive = false;

#ifdef DEBUG
        double oldTime = GetTime();
#endif // DEBUG

        // update matrix with the position of snake
        UpdateAppleMatrix(gameData->appleMatrix, &gameData->snakePlayer,
                          &gameData->gameField);

        // get random position and check with the matrix
        // limit the calculation by max N tries per cycle
        for (int i = 0; i < GET_APPLE_MAX_TRIES; i++)
        {
            isAppleValid = GetApple(&gameData->apple, &gameData->gameField,
                                    gameData->appleMatrix);
            if (isAppleValid)
            {
                gameData->dtApple = 0.0f;
                gameData->appleActive = true;
#ifdef DEBUG
                printf("apple found");
#endif // DEBUG
                break;
            }
        }
#ifdef DEBUG
        timeToGetApple = GetTime() - oldTime;
        printf("%f\n", timeToGetApple);

        // printf("IsAppleInSnake calls - %d\n", logIsAppleInSnake);
        // logIsAppleInSnake = 0;
#endif // DEBUG
    }
}

// mark snake coordinates with false
// in the matrix
void UpdateAppleMatrix(bool **appleMatrix, const Snake *snake,
                       const Rectangle *field)
{
    // reset apple matrix
    const size_t rows = FIELD_SIZE;
    const size_t cols = rows;
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            appleMatrix[i][j] = true;
        }
    }

    SnakeBlock *block = snake->tail;
    const float blockSize = block->body.height;
    while (block)
    {
        int row = (block->body.y - field->y) / blockSize;
        int col = (block->body.x - field->x) / blockSize;
        appleMatrix[row][col] = false;
        block = block->next;
    }
}
