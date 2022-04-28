#ifndef UTILS_H_SNAKE
#define UTILS_H_SNAKE

#include "raylib.h"

void UpdateGameField(float screenW, float screenH, Rectangle *gameField);
int GetBounds(Rectangle rec, float buf[4]);

#endif // !UTILS_H_SNAKE