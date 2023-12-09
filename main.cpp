#include "Game.hpp"
<<<<<<< HEAD
=======

<<<<<<< HEAD
#define N 20
#define HEIGHT N
#define WIDTH N
>>>>>>> parent of 355905d (split up main.cpp)

=======
>>>>>>> parent of af895d8 (Update main.cpp:Game(WIDTH, HEIGHT))
int main()
{

	cout << "Launching Snake Game..." << endl;
	InitWindow(2 * borderAllSides + cellCount * cellSize, 2 * borderAllSides + cellCount * cellSize, "Snake Game");
	SetTargetFPS(60);

	Game game = Game();
	game.Run();

	while (WindowShouldClose() == false)
	{
		UpdateMusicStream(game.music);

		BeginDrawing();

		if (eventTriggered(0.5))
		{
			game.Update();
		}

		int border = 4;
		DrawRectangleLinesEx(Rectangle{borderAllSides - border, borderAllSides - border, cellCount * cellSize + 2 * border, cellCount * cellSize + 2 * border}, border, game.gameRunning ? WHITE : GRAY);
		int fontSize = 28;
		DrawText("Snake Game", borderAllSides - border, 4, fontSize, game.gameRunning ? WHITE : GRAY);
		const char *scoreString = TextFormat("Score: %i", game.score);
		DrawText(scoreString, borderAllSides - border, borderAllSides + border + (cellCount * cellSize), fontSize, game.gameRunning ? WHITE : GRAY);

		if (game.gameRunning)
		{
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
			game.Draw();
		}
		else
		{
			DrawText("Game Over", borderAllSides + (cellCount * cellSize) / 2 - 80, borderAllSides + (cellCount * cellSize) / 2 - 32, 32, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();
	cout << "Game Ended." << endl;

	return 0;
}
