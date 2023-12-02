#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <deque>
#include <cstring>

using namespace std;

float cellSize = 32;
int cellCount = 8;
float borderAllSides = 32;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
	for (unsigned i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
		{
			return true;
		}
	}
	return false;
}

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

bool OutOfBound(Vector2 position)
{
	if (position.x >= cellCount || position.x <= -1 || position.y >= cellCount || position.y <= -1)
	{
		return true;
	}
	return false;
}

class Snake
{
public:
	deque<Vector2> bodyInit = {Vector2{6, 6}, Vector2{5, 6}, Vector2{4, 6}};
	deque<Vector2> body = bodyInit;
	deque<Vector2> bodyLastPosition = body;
	Vector2 directionInit = {1, 0};
	Vector2 direction = directionInit;
	bool bodyGrows = false;

	void Draw()
	{
		for (unsigned i = 0; i < body.size(); i++)
		{
			Vector2 position = body[i];
			float offset = borderAllSides;
			Rectangle segment = Rectangle{offset + (float)position.x * cellSize, offset + (float)position.y * cellSize, cellSize, cellSize};
			DrawRectangleRounded(segment, 0.5, 6, WHITE);
		}
	}

	void Update(bool gameRunning)
	{
		if (gameRunning)
		{
			bodyLastPosition = body;
			Vector2 nextPosition = Vector2Add(body[0], direction);
			if (OutOfBound(nextPosition) == false)
			{
				body.push_front(nextPosition);

				if (bodyGrows)
				{
					bodyGrows = false;
				}
				else
				{
					body.pop_back();
				}
			}
		}
	};
	void Reset()
	{
		body = bodyInit;
		direction = directionInit;
	}
	void SetToLastPosition()
	{
		body = bodyLastPosition;
	}
};

class Food
{
public:
	Vector2 position;
	Food(deque<Vector2> snakeBody)
	{
		position = GenerateRandomPos(snakeBody);
	};
	~Food(){};
	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{x, y};
	}
	Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
	{
		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, snakeBody))
		{
			position = GenerateRandomCell();
		}
		return position;
	};

	void Draw()
	{
		float offset = borderAllSides;
		DrawRectangle(offset + position.x * cellSize, offset + position.y * cellSize, cellSize, cellSize, WHITE);
	};
};

class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool gameRunning = true;
	int score = 0;
	Sound eatSound;
	Sound wallSound;

	Game()
	{
		InitAudioDevice();
		eatSound = LoadSound("Sound/eat.mp3");
		wallSound = LoadSound("Sound/wall.mp3");
	}
	~Game()
	{
		UnloadSound(eatSound);
		UnloadSound(wallSound);
		CloseAudioDevice();
	}

	void Draw()
	{
		food.Draw();
		snake.Draw();
	}
	void CheckCollisionWithEdges()
	{
		if (OutOfBound(snake.body[0]))
		{
			PlaySound(wallSound);
			GameOver();
		}
	}
	void CheckCollisionWithTail()
	{
		deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if (ElementInDeque(snake.body[0], headlessBody))
		{
			GameOver();
		}
	}
	void GameOver()
	{
		// snake.Reset();
		// food.position = food.GenerateRandomPos(snake.body);
		snake.SetToLastPosition();
		cout << "Game Over!" << endl;
		gameRunning = false;
		score = 0;
	}
	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomPos(snake.body);
			snake.bodyGrows = true;
			PlaySound(eatSound);
			score++;
		}
	}
	void Update()
	{
		if (gameRunning)
		{
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
			snake.Update(gameRunning);
			CheckCollisionWithFood();
		}
	}
};

int main()
{

	cout << "Launching Snake Game..." << endl;
	InitWindow(2 * borderAllSides + cellCount * cellSize, 2 * borderAllSides + cellCount * cellSize, "Snake Game");
	SetTargetFPS(60);

	Game game = Game();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		if (eventTriggered(0.5))
		{
			game.Update();
		}

		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
		{
			game.snake.direction = {0, -1};
		}
		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
		{
			game.snake.direction = {0, 1};
		}
		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
		{
			game.snake.direction = {-1, 0};
		}
		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
		{
			game.snake.direction = {1, 0};
		}

		ClearBackground(BLACK);
		int border = 4;
		DrawRectangleLinesEx(Rectangle{borderAllSides - border, borderAllSides - border, cellCount * cellSize + 2 * border, cellCount * cellSize + 2 * border}, border, WHITE);
		int fontSize = 30;
		DrawText("Snake Game", borderAllSides - border, 2, fontSize, WHITE);
		const char *scoreString = TextFormat("%i", game.score);
		DrawText(scoreString, cellCount * cellSize + borderAllSides - strlen(scoreString) * (fontSize - 20), 2, fontSize, WHITE);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();
	cout << "Game Ended." << endl;

	return 0;
}
