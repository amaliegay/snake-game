#pragma once

#include <raylib.h>

#include "Food.h"

class Snake {
   public:
    deque<Vector2> bodyInit = {Vector2{2, 1}, Vector2{1, 1}, Vector2{0, 1}};
    deque<Vector2> body = bodyInit;
    deque<Vector2> bodyLastPosition = body;
    Vector2 directionInit = {1, 0};
    Vector2 direction = directionInit;
    bool bodyGrows = false;
    bool running = true;

    void Draw(bool gameRunning) {
        for (unsigned i = 0; i < body.size(); i++) {
            Vector2 position = body[i];
            float offset = borderAllSides;
            Rectangle segment = Rectangle{offset + (float)position.x * cellSize,
                                          offset + (float)position.y * cellSize,
                                          cellSize, cellSize};
            DrawRectangleRounded(segment, 0.5, 6, gameRunning ? WHITE : GRAY);
        }
    }

    void Update(bool gameRunning) {
        if (gameRunning) {
            bodyLastPosition = body;
            Vector2 nextPosition = Vector2Add(body[0], direction);
            if (OutOfBound(nextPosition) == false) {
                body.push_front(nextPosition);

                if (bodyGrows) {
                    bodyGrows = false;
                } else {
                    body.pop_back();
                }
            }
        }
    };
    void Reset() {
        body = bodyInit;
        direction = directionInit;
    }
    void SetToLastPosition() { body = bodyLastPosition; }
};
