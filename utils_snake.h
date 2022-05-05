#ifndef UTILS_H_SNAKE
#define UTILS_H_SNAKE

#include "main.h"
#include "raylib.h"

void UpdateGameField(float screenW, float screenH, Rectangle *gameField,
                     float field_size);
int GetBounds(Rectangle rec, float buf[4]);
void LogCheckGameRatios(int snakeSize, int gameFieldWidth);
void InitGameData(GameData *gameData, float SCREEN_W, float SCREEN_H);
void CheckExitInput(GameData *gameData);
void AllocString(char **ptr, const char *msg);

#endif // !UTILS_H_SNAKE
