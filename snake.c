#include "raylib.h"
#include <stdio.h>

/* Defines */
// Colors
#define DARK_MAMBA                                                             \
    (Color) { 139, 120, 143, 1 }
// Numeric values
#define MAX_FPS (int)60
#define FIELD_SIZE (float)0.8
// String literals
#define TITLE (char *)"Snake"

// Game entities
typedef enum { UP = 0, DOWN, LEFT, RIGHT } DIRECTION;
typedef struct Snake {
    Rectangle *head;
    Rectangle *tail;
    float width;
    float height;
    DIRECTION direction : 2; // 0, 1, 2, 3 (four directions)
} Snake;

// void update

int main(void)
{
    // Config setup
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // Init window
    const int SCREEN_W = 1600;
    const int SCREEN_H = 900;
    InitWindow(SCREEN_W, SCREEN_H, TITLE);

    SetTargetFPS(MAX_FPS);
    SetExitKey(0);

    // Set window icon
    Image icon = LoadImage("../resources/icon.png");
    printf("----- %d\n", icon.format);
    SetWindowIcon(icon);

    Rectangle gameField = {0, 0, (float)SCREEN_W * FIELD_SIZE,
                           (float)SCREEN_H * FIELD_SIZE};
    gameField.x = (SCREEN_W - gameField.width) * 0.5f;
    gameField.y = (SCREEN_H - gameField.height) * 0.5f;

    // Game loop
    while (!WindowShouldClose())
    {
        // Set exit combination
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_Q))
        {
            CloseWindow();
        }

        // Update state
        if (IsWindowResized())
        {
            float screenW = (float)GetScreenWidth();
            float screenH = (float)GetScreenHeight();
            gameField = (Rectangle){0, 0, (float)screenW * FIELD_SIZE,
                                    (float)screenH * FIELD_SIZE};
            gameField.x = (screenW - gameField.width) * 0.5f;
            gameField.y = (screenH - gameField.height) * 0.5f;
        }

        // Draw
        BeginDrawing();

        ClearBackground(DARK_MAMBA);

        // Draw game field in the center of the screen
        DrawRectangleRec(gameField, GREEN);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}