#include <stdlib.h>

#include "main.h"

#include "raylib.h"
#include "snake.h"

void InitSnake(Snake *snake, const Rectangle *gameField,
               const int snakeBlockSize, SnakeBlock *snakeBlocks)
{
    Rectangle body = {(gameField->x + gameField->width * 0.5f) - snakeBlockSize,
                      gameField->y + gameField->height - snakeBlockSize,
                      snakeBlockSize, snakeBlockSize};
    snakeBlocks[0] = (SnakeBlock){body, NULL};
    snake->blockSize = snakeBlockSize;
    snake->head = &snakeBlocks[0];
    snake->tail = &snakeBlocks[0];
    snake->direction = UP;
}

void AddSnakeBlock(Rectangle *rec, Snake *snake)
{
    SnakeBlock newBlock = {*rec, NULL};
    snake->head->next = &newBlock;
    snake->head = &newBlock;
}

// hacky snake movement, the tail jumps to the next position
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
        } else
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
        } else
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
        } else
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
        } else
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

bool UpdateSnakeDirection(Snake *snake)
{
    if (IsKeyPressed(CONTROLS.snakeDown))
    {
        if (snake->direction != UP)
            snake->direction = DOWN;
        return true;
    } else if (IsKeyPressed(CONTROLS.snakeLeft))
    {
        if (snake->direction != RIGHT)
            snake->direction = LEFT;
        return true;
    } else if (IsKeyPressed(CONTROLS.snakeRight))
    {
        if (snake->direction != LEFT)
            snake->direction = RIGHT;
        return true;
    } else if (IsKeyPressed(CONTROLS.snakeUp))
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
