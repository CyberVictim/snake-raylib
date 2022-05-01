#ifndef SNAKE_H_SNAKE
#define SNAKE_H_SNAKE

#include "main.h"
#include "raylib.h"

void ResetGameData(GameData *gameData);
void RestartGame(GameData *gameData);
void UpdateSnake(GameData *gameData);
void InitSnake(Snake *snake, const Rectangle *gameField,
               const int snakeBlockSize, SnakeBlock *snakeBlock);
void DrawSnake(Snake *snake, Color bodyColor, Color headColor);
int UpdateSnakePosition(Snake *snake, const float bounds[4],
                        const Rectangle *apple);
bool UpdateSnakeDirection(Snake *snake, const Controls *controls);
void AddSnakeBlock(Rectangle *rec, Snake *snake);
void EatApple(Rectangle *apple, Snake *snake, SnakeBlock *appleBlock);

bool SnakeHitItself(const Snake *snake);
bool IsRecInSnake(const Rectangle *rec, const Snake *snake);

void GameOverScreenLoop(GameData *gameData);

#endif // !SNAKE_H_SNAKE
