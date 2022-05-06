#include "menu.h"
#include "raygui.h"
#include "raylib.h"
#include "utils_snake.h"
#include <stdlib.h>
#include <string.h>

void UpdateDrawMenuSettings(GameMenu *settingsMenu, GameData *gameData)
{
    BeginDrawing();

    ClearBackground(SNAKE_BACKGROUND_COLOR);
}

void InitSettingsMenu(SettingsMenu *settingsMenu, const int SCREEN_W,
                      const int SCREEN_H)
{
    int fontSize = (int)(SCREEN_H * 0.25f);

    char *group;
    char *changeRes;
    char *changeResValues;
    char *setSnakeUp;
    char *setSnakeDown;
    char *setSnakeRight;
    char *setSnakeLeft;
    char *checkFPS;
    char *checkFullScreen;

    AllocString(&changeRes, "Change resolution");
    AllocString(&changeResValues, "800x600;1280x720;1600x900;1920x1080");
    AllocString(&setSnakeUp, "Set snake up button");
    AllocString(&setSnakeDown, "Set snake down button");
    AllocString(&setSnakeRight, "Set snake right button");
    AllocString(&setSnakeLeft, "Set snake left button");
    AllocString(&checkFPS, "Show FPS");
    AllocString(&checkFullScreen, "Toggle Fullscreen");
    AllocString(&group, "SETTINGS");

    // init group area where all buttons go
    settingsMenu->group.rec = (Rectangle){SCREEN_W * 0.05f, SCREEN_H * 0.05,
                                          SCREEN_W * 0.9f, SCREEN_H * 0.9f};
    settingsMenu->group.name = group;

    // button values
    float butWidth = settingsMenu->group.rec.width * 0.6f;
    float butWSmall = settingsMenu->group.rec.width * 0.2f;
    float butHeight = settingsMenu->group.rec.height * 0.125f;
    // center aligned relative to the group
    float posX = settingsMenu->group.rec.x +
                 ((settingsMenu->group.rec.width - butWidth) * 0.5f);
    float posY = settingsMenu->group.rec.y + fontSize;
    // padding between buttons
    float pad = SCREEN_W * 0.05f;
    float paddingX = pad + butWSmall;
    float paddingY = pad + butHeight;

    // first row taking two columns
    // init change resolution label and dropbox with values
    settingsMenu->changeResolution.rec =
        (Rectangle){posX, posY, butWidth * 0.4f, butHeight};
    settingsMenu->changeResolution.name = changeRes;

    settingsMenu->changeResValues.rec =
        (Rectangle){posX + settingsMenu->changeResolution.rec.width + pad, posY,
                    settingsMenu->changeResolution.rec.width, butHeight};
    settingsMenu->changeResValues.name = changeResValues;

    // second row
    settingsMenu->checkFPS.rec =
        (Rectangle){posX, posY + paddingY, butWidth, butHeight};
    settingsMenu->checkFPS.name = checkFPS;

    // third row
    settingsMenu->checkFullScreen.rec =
        (Rectangle){posX, posY + paddingY * 2.0f, butWidth, butHeight};
    settingsMenu->checkFullScreen.name = checkFullScreen;

    // fourth row taking four columns
    // change snake controls 4 buttons
    settingsMenu->setSnakeUp.rec =
        (Rectangle){posX, posY + 3.0f * paddingY, butWSmall, butHeight};
    settingsMenu->setSnakeUp.name = setSnakeUp;

    settingsMenu->setSnakeDown.rec = (Rectangle){
        posX + paddingX, posY + 3.0f * paddingY, butWSmall, butHeight};
    settingsMenu->setSnakeDown.name = setSnakeDown;

    settingsMenu->setSnakeRight.rec = (Rectangle){
        posX + paddingX, posY + 3.0f * paddingY, butWSmall, butHeight};
    settingsMenu->setSnakeRight.name = setSnakeRight;

    settingsMenu->setSnakeLeft.rec = (Rectangle){
        posX + paddingX, posY + 3.0f * paddingY, butWSmall, butHeight};
    settingsMenu->setSnakeLeft.name = setSnakeLeft;
}

// draw immediate gui of main menu
void UpdateDrawMenu(GameMenu *gameMenu, GameData *gameData)
{
    BeginDrawing();

    ClearBackground(GetColor(CYAN_COLOR_S_HEX));
    GuiSetStyle(0, TEXT_SIZE, gameMenu->fontSize);

    GuiGroupBox(gameMenu->buttonGroup.rec, gameMenu->buttonGroup.name);

    if (GuiButton(gameMenu->bPlay.rec, gameMenu->bPlay.name))
    {
        gameData->GAME_STATE = GAME_ON;
    }

    if (GuiButton(gameMenu->bSettings.rec, gameMenu->bSettings.name))
    {
        gameData->GAME_STATE = GAME_MENU_SETTINGS;
    }

    if (GuiButton(gameMenu->bExit.rec, gameMenu->bExit.name))
    {
        gameData->GAME_STATE = GAME_EXIT;
    }

    EndDrawing();
}

// create rectangles needed to draw menu gui(raygui)
// allocates memory using raylib MemAlloc
void InitGameMenu(GameMenu *gameMenu, const int screenW, const int screenH)
{
    gameMenu->fontSize = (int)(screenH * 0.05f);

    char menuS[] = "MENU";
    char playS[] = "Play";
    char exitS[] = "Exit";
    char settingsS[] = "Settings";

    char *pMenuS = MemAlloc(sizeof(menuS));
    char *pPlayS = MemAlloc(sizeof(playS));
    char *pExitS = MemAlloc(sizeof(exitS));
    char *pSettingsS = MemAlloc(sizeof(settingsS));
    strcpy(pMenuS, menuS);
    strcpy(pPlayS, playS);
    strcpy(pExitS, exitS);
    strcpy(pSettingsS, settingsS);

    // init group where all buttons go
    gameMenu->buttonGroup.rec = (Rectangle){screenW * 0.05f, screenH * 0.05f,
                                            screenW * 0.4f, screenH * 0.9f};
    gameMenu->buttonGroup.name = pMenuS;

    //  button values
    float bWidth = gameMenu->buttonGroup.rec.width * 0.8f;
    float bHeight = gameMenu->buttonGroup.rec.height * 0.2f;
    float posX = gameMenu->buttonGroup.rec.x +
                 (gameMenu->buttonGroup.rec.width - bWidth) * 0.5f;
    float posY = gameMenu->buttonGroup.rec.y + gameMenu->fontSize;

    // padding between buttons
    float pad = 5.0f + bHeight;

    // first row, play button
    gameMenu->bPlay.rec = (Rectangle){posX, posY, bWidth, bHeight};
    gameMenu->bPlay.name = pPlayS;

    // second row, settings button
    gameMenu->bSettings.rec = (Rectangle){posX, posY + pad, bWidth, bHeight};
    gameMenu->bSettings.name = pSettingsS;

    // third row, exit button
    gameMenu->bExit.rec = (Rectangle){posX, posY + pad * 2.0f, bWidth, bHeight};
    gameMenu->bExit.name = pExitS;
}
