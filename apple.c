#include <stdlib.h>
#include <time.h>

#include "main.h"

#include "raylib.h"

#include "apple.h"


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
