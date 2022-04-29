#ifndef APPLE_H_SNAKE
#define APPLE_H_SNAKE

#include "main.h"
#include "raylib.h"

bool IsAppleInSnake(const Rectangle *apple, const Snake *snake, bool onlyHead);
void GetApple(Rectangle *apple, const Rectangle *gameField);

#endif // !APPLE_H_SNAKE