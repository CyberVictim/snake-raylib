#ifndef SNAKE_H_SNAKE
#define SNAKE_H_SNAKE

#include "main.h"
#include "raylib.h"

void InitSnake(Snake *snake, const Rectangle *gameField,
               const int snakeBlockSize, SnakeBlock *snakeBlocks);
void DrawSnake(Snake *snake, Color bodyColor, Color headColor);
int UpdateSnakePosition(Snake *snake, const float bounds[4], const Rectangle *apple);
bool UpdateSnakeDirection(Snake *snake);
void AddSnakeBlock(Rectangle *rec, Snake *snake);
void EatApple(Rectangle *apple, Snake *snake, SnakeBlock *appleBlock);

#endif // !SNAKE_H_SNAKE