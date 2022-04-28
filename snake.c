#include <stdlib.h>

#include "main.h"

#include "raylib.h"
#include "snake.h"

void AddSnakeBlock(Rectangle *rec, Snake *snake)
{
    SnakeBlock newBlock = {rec, NULL};
    snake->head->next = &newBlock;
    snake->head = &newBlock;
}
void UpdateSnakePosition(Snake *snake, const float bounds[4])
{
    // float dt = GetFrameTime();
    switch (snake->direction)
    {
    case UP:
        if (snake->head->body->y == bounds[1])
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y = bounds[3] - snake->tail->body->height;
        } else
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y =
                snake->head->body->y - snake->head->body->height;
        }
        break;
    case DOWN:
        if ((snake->head->body->y + snake->head->body->height) == bounds[3])
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y = bounds[1];
        } else
        {
            snake->tail->body->x = snake->head->body->x;
            snake->tail->body->y =
                snake->head->body->y + snake->head->body->height;
        }
        break;
    case LEFT:
        if (snake->head->body->x == bounds[0])
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x = bounds[2] - snake->tail->body->width;
        } else
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x =
                snake->head->body->x - snake->head->body->height;
        }
        break;
    case RIGHT:
        if ((snake->head->body->x + snake->head->body->width) == bounds[2])
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x = bounds[0];
        } else
        {
            snake->tail->body->y = snake->head->body->y;
            snake->tail->body->x =
                snake->head->body->x + snake->head->body->height;
        }
        break;
    }

    // If more than one snakeBlock, put tail in head
    if (!(snake->head == snake->tail))
    {
        SnakeBlock *tailCopy = snake->tail;
        snake->tail = tailCopy->next;
        tailCopy->next = NULL;
        snake->head->next = tailCopy;
        snake->head = tailCopy;
    }
}

int UpdateSnakeDirection(Snake *snake)
{
    if (IsKeyPressed(CONTROLS.snakeDown))
    {
        if (snake->direction != UP)
            snake->direction = DOWN;
        return 1;
    } else if (IsKeyPressed(CONTROLS.snakeLeft))
    {
        if (snake->direction != RIGHT)
            snake->direction = LEFT;
        return 1;
    } else if (IsKeyPressed(CONTROLS.snakeRight))
    {
        if (snake->direction != LEFT)
            snake->direction = RIGHT;
        return 1;
    } else if (IsKeyPressed(CONTROLS.snakeUp))
    {
        if (snake->direction != DOWN)
            snake->direction = UP;
        return 1;
    }
    return 0;
}

void EatApple(Rectangle *apple, Snake *snake, SnakeBlock *appleBlock)
{
    switch (snake->direction)
    {
    case UP:
        apple->x = snake->tail->body->x;
        apple->y = snake->tail->body->y + apple->width;
        break;
    case DOWN:
        apple->x = snake->tail->body->x;
        apple->y = snake->tail->body->y - apple->width;
        break;
    case LEFT:
        apple->x = snake->tail->body->x + apple->width;
        apple->y = snake->tail->body->y;
        break;
    case RIGHT:
        apple->x = snake->tail->body->x - apple->width;
        apple->y = snake->tail->body->y;
        break;
    }
    *appleBlock = (SnakeBlock){apple, snake->tail};
    snake->tail = appleBlock;
}

void DrawSnake(Snake *snake, Color color)
{
    // Start from tail if more than one block, otherwise just draw head
    SnakeBlock *block =
        (snake->head == snake->tail ? snake->head : snake->tail);
    while (block)
    {
        DrawRectangleRec(*block->body, color);
        block = block->next;
    }
}
