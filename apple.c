#include <stdlib.h>
#include <time.h>

#include "main.h"

#include "raylib.h"

#include "apple.h"

bool IsAppleInSnake(const Rectangle *rec, const Snake *snake, bool onlyHead)
{
    SnakeBlock *block = onlyHead ? snake->head : snake->tail;
    while (block)
    {
        if (rec->x == block->body->x && rec->y == block->body->y)
            return true;
        block = block->next;
    }
    return false;
}

void GetApple(int size, Rectangle **rec, const Rectangle *gameField)
{
    Rectangle apple;
    int rangeX = gameField->width / size - 1;
    int rangeY = gameField->height / size - 1;
    apple.height = apple.width = size;
    SetRandomSeed(time(NULL));
    int x = GetRandomValue(0, rangeX);
    int y = GetRandomValue(0, rangeY);
    apple.x = x * size + gameField->x;
    apple.y = y * size + gameField->y;
    *rec = MemAlloc(sizeof(Rectangle));
    **rec = apple;
}
