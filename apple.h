#ifndef APPLE_H_SNAKE
#define APPLE_H_SNAKE

#include "main.h"
#include "raylib.h"

bool IsAppleInSnake(const Rectangle *rec, const Snake *snake);
void GetApple(int size, Rectangle **rec, const Rectangle *gameField);

#endif // !APPLE_H_SNAKE