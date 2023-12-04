#pragma once

#include <raylib.h>

#include "Snake.h"

class Food {
   public:
    Vector2 position;
    Food(deque<Vector2> snakeBody) { position = GenerateRandomPos(snakeBody); };
    ~Food(){};
    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody)) {
            position = GenerateRandomCell();
        }
        return position;
    };

    void Draw(bool gameRunning) {
        float offset = borderAllSides;
        DrawRectangle(offset + position.x * cellSize,
                      offset + position.y * cellSize, cellSize, cellSize,
                      gameRunning ? WHITE : GRAY);
    };
};