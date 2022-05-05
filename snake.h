#ifndef SNAKE_H_SNAKE
#define SNAKE_H_SNAKE

#include "main.h"
#include "raylib.h"

void ResetGameData(GameData *gameData);
void RestartGame(GameData *gameData);

void InitSnake(Snake *snake, const Rectangle *gameField,
               const int snakeBlockSize, SnakeBlock *snakeBlock);
void InitGameMenu(GameMenu *gameMenu, const int screenW, const int screenH);

void DrawSnake(Snake *snake, Color bodyColor, Color headColor);
void DrawGame(GameData *gameData);

void UpdateDrawMenu(GameMenu *gameMenu, GameData *gameData);
void UpdateSnake(GameData *gameData);
int UpdateSnakePosition(Snake *snake, const float bounds[4],
                        const Rectangle *apple);
bool UpdateSnakeDirection(Snake *snake, const Controls *controls);

void EatApple(Rectangle *apple, Snake *snake, SnakeBlock *appleBlock);

bool SnakeHitItself(const Snake *snake);
bool IsRecInSnake(const Rectangle *rec, const Snake *snake);

#endif // !SNAKE_H_SNAKE
