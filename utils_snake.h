#ifndef UTILS_H_SNAKE
#define UTILS_H_SNAKE

#include "raylib.h"

void UpdateGameField(float screenW, float screenH, Rectangle *gameField,
                     float field_size);
int GetBounds(Rectangle rec, float buf[4]);
void LogCheckGameRatios(int snakeSize, int gameFieldWidth);

#endif // !UTILS_H_SNAKE
