#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <deque>

using namespace std;

float cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if(currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
	Vector2 direction = {1, 0};
    
    void Draw()
    {
        for (unsigned i = 0; i < body.size(); i++)
        {
            Vector2 position = body[i];
            Rectangle segment = Rectangle{(float)position.x * cellSize, (float)position.y * cellSize, cellSize, cellSize};
            DrawRectangleRounded(segment, 0.5, 6, WHITE);
        }
    }

	void Update()
	{
		body.pop_back();
		body.push_front(Vector2Add(body[0] + direction));
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

class Game
{
public:
	Snake snake = Snake();
	Food food = Food();

	void Draw()
{
	food.Draw();
	snake.Draw();
}
	void Update()
{
	snake.Update();
}
}

int main()
{

    cout << "Launching Snake Game..." << endl;
    InitWindow(cellCount * cellSize, cellCount * cellSize, "Snake Game");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

		if(eventTriggered(0.2))
		{
			game.Update();
		}

		if(IsKeyPressed(KEY_UP) && snake.direction.y != 1)
		{
			snake.direction = {0, -1};
		}
		if(IsKeyPressed(KEY_DOWN) && snake.direction.y != -1)
		{
			snake.direction = {0, 1};
		}
		if(IsKeyPressed(KEY_LEFT) && snake.direction.x != 1)
		{
			snake.direction = {-1, 0};
		}
		if(IsKeyPressed(KEY_RIGHT) && snake.direction.y != -1)
		{
			snake.direction = {1, 0};
		}

        food.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
