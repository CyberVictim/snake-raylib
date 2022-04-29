#include <stdlib.h>
#include <time.h>

#include "main.h"

#include "raylib.h"

#include "apple.h"

bool IsAppleInSnake(const Rectangle *apple, const Snake *snake, bool onlyHead)
{
    SnakeBlock *block = onlyHead ? snake->head : snake->tail;
    while (block)
    {
        if (apple->x == block->body.x && apple->y == block->body.y)
            return true;
        block = block->next;
    }
    return false;
}

void GetApple(Rectangle *apple, const Rectangle *gameField)
{
    // Get range for possible fixed coordinates (0 to MAX)
    int appleSize = apple->height;
    int rangeX = gameField->width / appleSize - 1;
    int rangeY = gameField->height / appleSize - 1;

    // Get pseudo random fixed coordinate from range
    SetRandomSeed(time(NULL));
    int x = GetRandomValue(0, rangeX);
    int y = GetRandomValue(0, rangeY);

    // Calculate actual screen coordinate
    apple->x = x * appleSize + gameField->x;
    apple->y = y * appleSize + gameField->y;
}
