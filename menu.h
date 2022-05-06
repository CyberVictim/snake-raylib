#ifndef MENU_H_SNAKE
#define MENU_H_SNAKE

#include "main.h"

void InitSettingsMenu(SettingsMenu *settingsMenu, const int SCREEN_W,
                      const int SCREEN_H);
void UpdateDrawMenuSettings(GameMenu *settingsMenu, GameData *gameData);

void InitGameMenu(GameMenu *gameMenu, const int screenW, const int screenH);
void UpdateDrawMenu(GameMenu *gameMenu, GameData *gameData);

#endif // !MENU_H_SNAKE
