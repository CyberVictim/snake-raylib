#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "menu.h"
#include "raygui.h"
#include "raylib.h"
#include "snake.h"
#include "utils_snake.h"

bool SnakeHitItself(const Snake *snake)
{
    float posX = snake->head->body.x;
    float posY = snake->head->body.y;
    SnakeBlock *block = snake->tail;
    while (block)
    {
        if (posX == block->body.x && posY == block->body.y)
        {
            if (block->next)
            {
                return true;
            }
            else
            {
                break;
            }
        }
        block = block->next;
    }
    return false;
}
bool IsRecInSnake(const Rectangle *rec, const Snake *snake)
{
    SnakeBlock *block = snake->tail;
    while (block)
    {
        if (rec->x == block->body.x && rec->y == block->body.y)
        {
            return true;
        }
        block = block->next;
    }
    return false;
}
void InitSnake(Snake *snake, const Rectangle *gameField,
               const int snakeBlockSize, SnakeBlock *snakeBlock)
{
    Rectangle body = {(gameField->x + gameField->width * 0.5f) - snakeBlockSize,
                      gameField->y + gameField->height - snakeBlockSize,
                      snakeBlockSize, snakeBlockSize};
    *snakeBlock = (SnakeBlock){body, NULL};
    snake->head = snakeBlock;
    snake->tail = snakeBlock;
    snake->direction = UP;
}

// snake movement, the tail jumps to the next position
int UpdateSnakePosition(Snake *snake, const float bounds[4],
                        const Rectangle *apple)
{
    float newPosX, newPosY;
    switch (snake->direction)
    {
    case UP:
        if (snake->head->body.y == bounds[1])
        {
            newPosX = snake->head->body.x;
            newPosY = bounds[3] - snake->tail->body.height;
        }
        else
        {
            newPosX = snake->head->body.x;
            newPosY = snake->head->body.y - snake->head->body.height;
        }
        break;
    case DOWN:
        if ((snake->head->body.y + snake->head->body.height) == bounds[3])
        {
            newPosX = snake->head->body.x;
            newPosY = bounds[1];
        }
        else
        {
            newPosX = snake->head->body.x;
            newPosY = snake->head->body.y + snake->head->body.height;
        }
        break;
    case LEFT:
        if (snake->head->body.x == bounds[0])
        {
            newPosY = snake->head->body.y;
            newPosX = bounds[2] - snake->tail->body.width;
        }
        else
        {
            newPosY = snake->head->body.y;
            newPosX = snake->head->body.x - snake->head->body.height;
        }
        break;
    case RIGHT:
        if ((snake->head->body.x + snake->head->body.width) == bounds[2])
        {
            newPosY = snake->head->body.y;
            newPosX = bounds[0];
        }
        else
        {
            newPosY = snake->head->body.y;
            newPosX = snake->head->body.x + snake->head->body.height;
        }
        break;
    }

    // check if snake eats apple
    if (apple)
    {
        if (newPosX == apple->x && newPosY == apple->y)
            return 1;
    }

    // update coordinates
    snake->tail->body.x = newPosX;
    snake->tail->body.y = newPosY;

    // If more than one snakeBlock, put tail in head
    if (!(snake->head == snake->tail))
    {
        SnakeBlock *tailCopy = snake->tail;
        snake->tail = tailCopy->next;
        tailCopy->next = NULL;
        snake->head->next = tailCopy;
        snake->head = tailCopy;
    }
    return 0;
}

bool UpdateSnakeDirection(Snake *snake, const Controls *controls)
{
    if (IsKeyPressed(controls->snakeDown))
    {
        if (snake->direction != UP)
            snake->direction = DOWN;
        return true;
    }
    else if (IsKeyPressed(controls->snakeLeft))
    {
        if (snake->direction != RIGHT)
            snake->direction = LEFT;
        return true;
    }
    else if (IsKeyPressed(controls->snakeRight))
    {
        if (snake->direction != LEFT)
            snake->direction = RIGHT;
        return true;
    }
    else if (IsKeyPressed(controls->snakeUp))
    {
        if (snake->direction != DOWN)
            snake->direction = UP;
        return true;
    }
    return false;
}

void EatApple(Rectangle *apple, Snake *snake, SnakeBlock *appleBlock)
{
    *appleBlock = (SnakeBlock){*apple, NULL};
    snake->head->next = appleBlock;
    snake->head = appleBlock;
}

void DrawSnake(Snake *snake, Color bodyColor, Color headColor)
{
    // Start from tail if more than one block, otherwise just draw head
    SnakeBlock *block =
        (snake->head == snake->tail ? snake->head : snake->tail);
    while (block)
    {
        DrawRectangleRec(block->body, bodyColor);
        block = block->next;
    }
    DrawRectangleRec(snake->head->body, headColor);
}

// Update snakePlayer
void UpdateSnake(GameData *gameData)
{
    gameData->dtSnake += GetFrameTime();

    // if enough time has passed or button has been pressed
    if (UpdateSnakeDirection(&gameData->snakePlayer, &gameData->CONTROLS) ||
        gameData->dtSnake >= SNAKE_SPEED)
    {
        gameData->dtSnake = 0.0f;
        float Fieldbounds[4];
        GetBounds(gameData->gameField, Fieldbounds);

        // Update position and process eating apple if function returns 1
        if (UpdateSnakePosition(&gameData->snakePlayer, Fieldbounds,
                                gameData->appleActive ? &gameData->apple
                                                      : NULL))
        {
            EatApple(&gameData->apple, &gameData->snakePlayer,
                     &gameData->snakeBlocks[gameData->blocksCounter++]);

            // if snake filled the whole game screen
            if (gameData->blocksCounter == gameData->maxBlocks)
            {
                gameData->GAME_STATE = GAME_SCREEN_FILLED;
#ifdef DEBUG
                printf("game field filled\n");
#endif // DEBUG
            }

            gameData->appleActive = false;
            gameData->dtApple = APPLE_SPEED - APPLE_SPEED_FIRST;
        }
        if (SnakeHitItself(&gameData->snakePlayer))
        {
            gameData->GAME_STATE = GAME_OVER;
#ifdef DEBUG
            printf("game over\n");
#endif // DEBUG
        }
    }
}

void UpdateResolution(GameData *gameData, GameMenu *gameMenu,
                      SettingsMenu *settingsMenu, const int resolutionId)
{
    ChangeSnakeWindowSize(resolutionId);

    gameData->blockSize = GetScreenHeight() / SNAKE_SIZE;
    gameData->apple.height = gameData->apple.width = gameData->blockSize;
    UpdateGameField(&gameData->gameField, gameData->blockSize);
    gameData->blocksCounter = 0;
    InitSnake(&gameData->snakePlayer, &gameData->gameField, gameData->blockSize,
              &gameData->snakeBlocks[gameData->blocksCounter++]);

    UpdateGameMenuButtons(gameMenu);
    UpdateSettingsMenuButtons(settingsMenu);
}

void ResetGameData(GameData *gameData)
{
    free(gameData->alertMsg);
    gameData->alertMsg = NULL;
    gameData->GAME_STATE = INITIAL_GAME_STATE;
    gameData->appleActive = false;
    gameData->dtApple = APPLE_SPEED - APPLE_SPEED_FIRST;
    gameData->dtSnake = 0.0f;
    gameData->blocksCounter = 0;
    InitSnake(&gameData->snakePlayer, &gameData->gameField, gameData->blockSize,
              &gameData->snakeBlocks[gameData->blocksCounter++]);
}

void DrawGame(GameData *gameData)
{
    BeginDrawing();

    // if (gameData->gameSettingsFlags & SET_SNAKE_SHOW_FPS)
    // {
    //     DrawFPS(15, 15);
    // }

    switch (gameData->GAME_STATE)
    {
    case GAME_ON:
        ClearBackground(SNAKE_BACKGROUND_COLOR);
        DrawRectangleRec(gameData->gameField, SOFT_GREEN);
        DrawSnake(&gameData->snakePlayer, SNAKE_BODY_COLOR, SNAKE_HEAD_COLOR);

        if (gameData->appleActive)
            DrawRectangleRec(gameData->apple, BANANA);
        break;

    case GAME_MENU:
        break;

    case GAME_OVER:
    case GAME_SCREEN_FILLED:
        ClearBackground(GetColor(BROWN_COLOR_S_HEX));

        float bigFontSize = gameData->gameField.height * 0.1f;
        DrawText(gameData->alertMsg, gameData->gameField.width * 0.2f, 20,
                 bigFontSize, BANANA);

        DrawText("Press ENTER to restart, ESCAPE for menu", 20,
                 bigFontSize + 30, gameData->gameField.height * 0.06f,
                 SOFT_RED);

        if (IsKeyPressed(KEY_ENTER))
        {
            RestartGame(gameData);
        }
        else if (IsKeyPressed(KEY_ESCAPE))
        {
            ResetGameData(gameData);
        }
        break;

    default:
        break;
    }

    EndDrawing();
}

void RestartGame(GameData *gameData)
{
    ResetGameData(gameData);
    gameData->GAME_STATE = GAME_ON;
}
