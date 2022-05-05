#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "apple.h"
#include "main.h"
#include "raylib.h"
#include "snake.h"

static void GetApple(Rectangle *apple, const Rectangle *gameField)
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

void UpdateApple(GameData *gameData)
{
    gameData->dtApple += GetFrameTime();

    // if enough time has passed
    if ((gameData->dtApple >= APPLE_SPEED &&
         gameData->GAME_STATE != GAME_SCREEN_FILLED))
    {
        // pause time if we don't find apple in this frame
        gameData->dtApple = APPLE_SPEED - 0.1f;
        bool appleInSnake = true;
        gameData->appleActive = false;

        // get random position and check collisions
        // limit the calculation by max N tries per cycle
        for (int i = 0; i < GET_APPLE_MAX_TRIES; i++)
        {
            GetApple(&gameData->apple, &gameData->gameField);
            appleInSnake =
                IsRecInSnake(&gameData->apple, &gameData->snakePlayer);

#ifdef DEBUG
            // counting the number of function calls
            logIsAppleInSnake++;
#endif // DEBUG

            // if found valid apple coordinates
            if (!appleInSnake)
            {
                gameData->dtApple = 0.0f;
                gameData->appleActive = true;
                break;
            }
        }

#ifdef DEBUG
        printf("IsAppleInSnake calls - %d\n", logIsAppleInSnake);
        logIsAppleInSnake = 0;
#endif // DEBUG
    }
}
