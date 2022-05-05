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
void DrawGame(GameData *gameData);
void InitGameMenu(GameMenu *gameMenu, const int screenW, const int screenH);
void UpdateDrawMenu(GameMenu *gameMenu, GameData *gameData);
bool UpdateSnakeDirection(Snake *snake, const Controls *controls);
void AddSnakeBlock(Rectangle *rec, Snake *snake);
void EatApple(Rectangle *apple, Snake *snake, SnakeBlock *appleBlock);

bool SnakeHitItself(const Snake *snake);
bool IsRecInSnake(const Rectangle *rec, const Snake *snake);

void GameStateAlert(GameData *gameData, const char *msg, int screen_h);

#endif // !SNAKE_H_SNAKE
