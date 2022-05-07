#include "menu.h"
#include "raygui.h"
#include "raylib.h"
#include "utils_snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void UpdateDrawMenuSettings(SettingsMenu *menu, GameData *gameData)
{
    BeginDrawing();

    ClearBackground(SNAKE_BACKGROUND_COLOR);

    if (GetKeyPressed() == KEY_ESCAPE)
    {
        gameData->GAME_STATE = GAME_MENU;
    }

    // lock gui if dropdown box is active
    menu->changeResEditMode ? GuiLock() : GuiUnlock();
    // if (menu->changeResEditMode)
    // {
    //     GuiLock();
    // }
    // else
    // {
    //     GuiUnlock();
    // }

    GuiSetStyle(0, TEXT_SIZE, menu->fontSize);
    GuiGroupBox(menu->group.rec, menu->group.name);

    // GuiSetStyle(0, TEXT_SIZE,menu->fontSize);
    GuiGroupBox(menu->groupControls.rec, menu->groupControls.name);

    if (GuiButton(menu->changeResolution.rec, menu->changeResolution.name))
    {
        // format the string with the button name
        char msg[] =
            "Choose resolution from dropdown and to save press the button ";
        char statusMsg[strlen(msg) + strlen(menu->save.name) + 1];
        sprintf(statusMsg, "%s%s", msg, menu->save.name);

        GuiStatusBar(
            (Rectangle){0, GetScreenHeight() - 20, GetScreenWidth(), 20},
            statusMsg);
    }

    menu->checkFPSBool = GuiCheckBox(menu->checkFPS.rec, menu->checkFPS.name,
                                     menu->checkFPSBool);

    menu->checkFullScreenBool =
        GuiCheckBox(menu->checkFullScreen.rec, menu->checkFullScreen.name,
                    menu->checkFullScreenBool);

    // control change buttons will save the key being changed
    // and change game state to controls set menu
    if (GuiButton(menu->setSnakeDown.rec, menu->setSnakeDown.name))
    {
        gameData->GAME_STATE = GAME_SET_CONTROLS;
        gameData->CONTROLS.changedKey = &gameData->CONTROLS.snakeDown;
    }

    if (GuiButton(menu->setSnakeLeft.rec, menu->setSnakeLeft.name))
    {
        gameData->GAME_STATE = GAME_SET_CONTROLS;
        gameData->CONTROLS.changedKey = &gameData->CONTROLS.snakeLeft;
    }

    if (GuiButton(menu->setSnakeRight.rec, menu->setSnakeRight.name))
    {
        gameData->GAME_STATE = GAME_SET_CONTROLS;
        gameData->CONTROLS.changedKey = &gameData->CONTROLS.snakeRight;
    }

    if (GuiButton(menu->setSnakeUp.rec, menu->setSnakeUp.name))
    {
        gameData->GAME_STATE = GAME_SET_CONTROLS;
        gameData->CONTROLS.changedKey = &gameData->CONTROLS.snakeUp;
    }

    // apply the changes
    if (GuiButton(menu->save.rec, menu->save.name))
    {
        // toggle full screen
        if (menu->checkFullScreenBool)
        {
            if (!IsWindowFullscreen())
            {
                ToggleFullscreen();
            }
        }
        else
        {
            if (IsWindowFullscreen())
            {
                ToggleFullscreen();
            }
        }

        // toggle fps counter
        menu->checkFPSBool
            ? (gameData->gameSettingsFlags |= SET_SNAKE_SHOW_FPS)
            : (gameData->gameSettingsFlags &= ~SET_SNAKE_SHOW_FPS);
        // if (menu->checkFPSBool)
        // {
        //     gameData->gameSettingsFlags |= SET_SNAKE_SHOW_FPS;
        // }
        // else
        // {
        //     gameData->gameSettingsFlags &= ~SET_SNAKE_SHOW_FPS;
        // }

        // apply resolution
        switch (menu->changeResBoxValue)
        {
        case 0:
            SetWindowSize(800, 600);
            break;

        case 1:
            SetWindowSize(1280, 720);
            break;

        case 2:
            SetWindowSize(1600, 900);
            break;

        case 3:
            SetWindowSize(1920, 1080);
            break;

        default:
            break;
        }
    }

    // has to be drawn after the elements
    // that are below it on the screen
    if (GuiDropdownBox(menu->changeResValues.rec, menu->changeResValues.name,
                       &menu->changeResBoxValue, menu->changeResEditMode))
    {
        menu->changeResEditMode = !menu->changeResEditMode;
    }

    EndDrawing();
}

void InitSettingsMenu(SettingsMenu *settingsMenu, const int SCREEN_W,
                      const int SCREEN_H)
{
    settingsMenu->fontSize = (int)(SCREEN_H * 0.04f);
    settingsMenu->changeResEditMode = false;
    settingsMenu->changeResBoxValue = 0;
    settingsMenu->checkFPSBool = false;
    settingsMenu->checkFullScreenBool = false;

    char *group;
    char *changeRes;
    char *changeResValues;
    char *setSnakeUp;
    char *setSnakeDown;
    char *setSnakeRight;
    char *setSnakeLeft;
    char *checkFPS;
    char *checkFullScreen;
    char *save;
    char *groupControls;

    AllocString(&groupControls, "CHANGE CONTROLS");
    AllocString(&changeRes, "Change resolution");
    AllocString(&save, "Apply changes");
    AllocString(&changeResValues, "800x600;1280x720;1600x900;1920x1080");
    AllocString(&setSnakeUp, "Snake up");
    AllocString(&setSnakeDown, "Snake down");
    AllocString(&setSnakeRight, "Snake right");
    AllocString(&setSnakeLeft, "Snake left");
    AllocString(&checkFPS, "Show FPS");
    AllocString(&checkFullScreen, "Toggle Fullscreen");
    AllocString(&group, "SETTINGS");

    // init group area where all buttons go
    settingsMenu->group.rec = (Rectangle){SCREEN_W * 0.02f, SCREEN_H * 0.03f,
                                          SCREEN_W * 0.95f, SCREEN_H * 0.95f};
    settingsMenu->group.name = group;

    // button values
    float butWidth = settingsMenu->group.rec.width * 0.6f;
    float butWSmall = settingsMenu->group.rec.width * 0.2f;
    float butHeight = settingsMenu->group.rec.height * 0.125f;
    float checkBoxHeight = butHeight * 0.5f;
    float posX =
        settingsMenu->group.rec.x + settingsMenu->group.rec.width * 0.02f;
    float posY = settingsMenu->group.rec.y + (float)(settingsMenu->fontSize);
    // padding between buttons
    float pad = SCREEN_W * 0.05f;
    float paddingX = pad + butWSmall;
    float paddingY = pad + butHeight;

    // first row taking two columns
    // init change resolution label and dropbox with values
    settingsMenu->changeResolution.rec =
        (Rectangle){posX, posY, butWidth * 0.8f, butHeight};
    settingsMenu->changeResolution.name = changeRes;

    settingsMenu->changeResValues.rec =
        (Rectangle){posX + settingsMenu->changeResolution.rec.width + pad, posY,
                    butWSmall * 1.2f, butHeight};
    settingsMenu->changeResValues.name = changeResValues;

    // second row, two columns

    settingsMenu->checkFPS.rec =
        (Rectangle){posX, posY + paddingY, checkBoxHeight, checkBoxHeight};
    settingsMenu->checkFPS.name = checkFPS;

    settingsMenu->checkFullScreen.rec = (Rectangle){
        posX + paddingX, posY + paddingY, checkBoxHeight, checkBoxHeight};
    settingsMenu->checkFullScreen.name = checkFullScreen;

    // third row taking four columns
    // inner group
    settingsMenu->groupControls.rec = (Rectangle){
        settingsMenu->group.rec.x + settingsMenu->group.rec.width * 0.005f,
        posY + 2.0f * paddingY, settingsMenu->group.rec.width * 0.99f,
        butHeight * 1.3f};
    settingsMenu->groupControls.name = groupControls;

    float controlsPosX = posX + settingsMenu->groupControls.rec.x * 0.01f;
    float controlsPosY =
        settingsMenu->groupControls.rec.y + (float)settingsMenu->fontSize;

    settingsMenu->setSnakeUp.rec =
        (Rectangle){controlsPosX, controlsPosY, butWSmall, butHeight * 0.6f};
    settingsMenu->setSnakeUp.name = setSnakeUp;

    settingsMenu->setSnakeDown.rec = (Rectangle){
        controlsPosX + paddingX, controlsPosY, butWSmall, butHeight * 0.6f};
    settingsMenu->setSnakeDown.name = setSnakeDown;

    settingsMenu->setSnakeRight.rec =
        (Rectangle){controlsPosX + 2.0f * paddingX, controlsPosY, butWSmall,
                    butHeight * 0.6f};
    settingsMenu->setSnakeRight.name = setSnakeRight;

    settingsMenu->setSnakeLeft.rec =
        (Rectangle){controlsPosX + 3.0f * paddingX, controlsPosY, butWSmall,
                    butHeight * 0.6f};
    settingsMenu->setSnakeLeft.name = setSnakeLeft;

    // fourth row
    settingsMenu->save.rec =
        (Rectangle){posX + settingsMenu->group.rec.width * 0.3f,
                    posY + paddingY * 3.0f, butWSmall * 1.3f, butHeight};
    settingsMenu->save.name = save;
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

void FreeSettingsMenu(SettingsMenu *settingsMenu)
{
    free(settingsMenu->changeResolution.name);
    free(settingsMenu->groupControls.name);
    free(settingsMenu->changeResValues.name);
    free(settingsMenu->checkFPS.name);
    free(settingsMenu->checkFullScreen.name);
    free(settingsMenu->group.name);
    free(settingsMenu->setSnakeDown.name);
    free(settingsMenu->setSnakeLeft.name);
    free(settingsMenu->setSnakeRight.name);
    free(settingsMenu->setSnakeUp.name);
    free(settingsMenu->save.name);
}

void ReadUpdateControls(SnakeGameState *gameState, KeyboardKey *changedKey)
{
    BeginDrawing();

    ClearBackground(BANANA);

    DrawText("Press keyboard button to save\n escape to cancel", 5, 5,
             GetScreenHeight() * 0.06f, SOFT_RED);
    KeyboardKey currentKey = GetKeyPressed();
    if (currentKey)
    {
        if (currentKey != KEY_ESCAPE)
        {
            *changedKey = currentKey;
        }
        *gameState = GAME_MENU_SETTINGS;
    }

    EndDrawing();
}
