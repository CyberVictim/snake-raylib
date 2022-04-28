#include "main.h"
#include "raylib.h"
// Relative to the screen
void UpdateGameField(float screenW, float screenH, Rectangle *gameField)
{
    gameField->width = screenW * FIELD_SIZE;
    gameField->height = screenH * FIELD_SIZE;
    gameField->x = (screenW - gameField->width) * 0.5f;
    gameField->y = (screenH - gameField->height) * 0.5f;
}

// Fill the array with bounds of the rectangle, starting from left side going
// clockwise
int GetBounds(Rectangle rec, float buf[4])
{
    buf[0] = rec.x;
    buf[1] = rec.y;
    buf[2] = rec.x + rec.width;
    buf[3] = rec.y + rec.height;
    return 0;
}
