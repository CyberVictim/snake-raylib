#ifndef MAIN_H_SNAKE
#define MAIN_H_SNAKE

#include "raylib.h"

/* Defines */

// Colors
#define DARK_MAMBA                                                             \
    (Color) { 139, 120, 143, 255 }
#define SOFT_RED                                                               \
    (Color) { 195, 89, 89, 255 }
#define SOFT_GREEN                                                             \
    (Color) { 135, 193, 155, 255 }
#define BANANA                                                                 \
    (Color) { 214, 222, 146, 255 }

// Numeric values
#define MAX_FPS 120
#define FIELD_SIZE 0.8f  // % of the screen
#define SNAKE_SIZE 0.05f // % of the screen
#define SNAKE_SPEED 0.3f // in seconds
#define APPLE_SPEED 8.0f // in seconds

// String literals
#define TITLE (char *)"Snake"

// Game entities
typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;

typedef struct SnakeBlock {
    Rectangle body;
    struct SnakeBlock *next;
} SnakeBlock;

typedef struct Snake {
    float blockSize;
    SnakeBlock *head;
    SnakeBlock *tail;
    DIRECTION direction : 3; // 0, 1, 2, 3 (four directions)
    // SnakeBlock blocks[];
} Snake;

typedef struct Controls {
    KeyboardKey snakeUp;
    KeyboardKey snakeDown;
    KeyboardKey snakeLeft;
    KeyboardKey snakeRight;
} Controls;

extern Controls CONTROLS;

#endif // !MAIN_H_SNAKE