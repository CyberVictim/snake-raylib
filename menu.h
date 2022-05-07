#ifndef MENU_H_SNAKE
#define MENU_H_SNAKE

#include "main.h"
#include "raylib.h"

void InitSettingsMenu(SettingsMenu *settingsMenu, const int SCREEN_W,
                      const int SCREEN_H);
void UpdateDrawMenuSettings(SettingsMenu *menu, GameData *gameData);

void InitGameMenu(GameMenu *gameMenu, const int screenW, const int screenH);
void UpdateDrawMenu(GameMenu *gameMenu, GameData *gameData);

void FreeSettingsMenu(SettingsMenu *settingsMenu);

void ReadUpdateControls(SnakeGameState *gameState, KeyboardKey *changedKey);

#endif // !MENU_H_SNAKE
