#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <deque>

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
	deque<Vector2> bodyInit = {Vector2{2, 1}, Vector2{1, 1}, Vector2{0, 1}};
	deque<Vector2> body = bodyInit;
	deque<Vector2> bodyLastPosition = body;
	Vector2 directionInit = {1, 0};
	Vector2 direction = directionInit;
	bool bodyGrows = false;
	bool running = true;

	void Draw(bool gameRunning)
	{
		for (unsigned i = 0; i < body.size(); i++)
		{
			Vector2 position = body[i];
			float offset = borderAllSides;
			Rectangle segment = Rectangle{offset + (float)position.x * cellSize, offset + (float)position.y * cellSize, cellSize, cellSize};
			DrawRectangleRounded(segment, 0.5, 6, gameRunning ? WHITE : GRAY);
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

	void Draw(bool gameRunning)
	{
		float offset = borderAllSides;
		DrawRectangle(offset + position.x * cellSize, offset + position.y * cellSize, cellSize, cellSize, gameRunning ? WHITE : GRAY);
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
	Sound tailSound;
	Music music;

	Game()
	{
		InitAudioDevice();
		eatSound = LoadSound("Sound/eat.wav");
		wallSound = LoadSound("Sound/wall.wav");
		tailSound = LoadSound("Sound/tail.wav");
		music = LoadMusicStream("Music/Snake Game.wav");
		PlayMusicStream(music);
		SetMusicVolume(music, 0.5);
	}
	~Game()
	{
		UnloadSound(eatSound);
		UnloadSound(tailSound);
		UnloadSound(wallSound);
		CloseAudioDevice();
	}

	void Draw()
	{
		food.Draw(gameRunning);
		snake.Draw(gameRunning);
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
			PlaySound(tailSound);
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
		// score = 0;
		StopMusicStream(music);
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
}
