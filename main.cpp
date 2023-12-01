#include <iostream>
#include <raylib.h>
#include <deque>

using namespace std;

float cellSize = 30;
int cellCount = 25;

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};

    void Draw()
    {
        for (unsigned i = 0; i < body.size(); i++)
        {
            Vector2 position = body[i];
            Rectangle segment = Rectangle{(float)position.x * cellSize, (float)position.y * cellSize, cellSize, cellSize};
            DrawRectangleRounded(segment, 0.5, 6, WHITE);
        }
    }
};

class Food
{
private:
public:
    Vector2 GenerateRandomPos()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 position = GenerateRandomPos();

    void Draw()
    {
        DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, WHITE);
    }
};

int main()
{

    cout << "Launching Snake Game..." << endl;
    InitWindow(cellCount * cellSize, cellCount * cellSize, "Snake Game");
    SetTargetFPS(60);

    Food food = Food();
    Snake snake = Snake();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        food.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}