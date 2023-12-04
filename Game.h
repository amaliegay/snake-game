#pragma once

#include <raylib.h>

#include "Snake.h"
#include "Food.h"

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
	int width;
	int height;

	Game(int WIDTH, int HEIGHT);
	~Game();
	void Run();
	void Draw();
	void CheckCollisionWithEdges();
	void CheckCollisionWithTail();
	void GameOver();
	void CheckCollisionWithFood();
	void Update();
}
