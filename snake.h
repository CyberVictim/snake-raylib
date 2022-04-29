#ifndef SNAKE_H_SNAKE
#define SNAKE_H_SNAKE

#include "main.h"
#include "raylib.h"

void InitSnake(Snake *snake, const Rectangle *gameField,
               const int snakeBlockSize, SnakeBlock *snakeBlocks);
void DrawSnake(Snake *snake, Color color);
void UpdateSnakePosition(Snake *snake, const float bounds[4]);
int UpdateSnakeDirection(Snake *snake);
void AddSnakeBlock(Rectangle *rec, Snake *snake);
void EatApple(Rectangle *apple, Snake *snake, SnakeBlock *appleBlock);

#endif // !SNAKE_H_SNAKE