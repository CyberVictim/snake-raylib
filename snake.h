#ifndef SNAKE_H_SNAKE
#define SNAKE_H_SNAKE

#include "main.h"

void DrawSnake(Snake *snake, Color color);
void UpdateSnakePosition(Snake *snake, const float bounds[4]);
int UpdateSnakeDirection(Snake *snake);
void AddSnakeBlock(Rectangle *rec, Snake *snake);

#endif // !SNAKE_H_SNAKE