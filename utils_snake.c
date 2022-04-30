#include "utils_snake.h"
#include "main.h"
#include "raylib.h"
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
