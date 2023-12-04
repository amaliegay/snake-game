#include <raylib.h>
#include <raymath.h>

#include <deque>
#include <iostream>

#include "Game.h"

using namespace std;

float cellSize = 32;
int cellCount = 8;
float borderAllSides = 32;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for (unsigned i = 0; i < deque.size(); i++) {
        if (Vector2Equals(deque[i], element)) {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool OutOfBound(Vector2 position) {
    if (position.x >= cellCount || position.x <= -1 ||
        position.y >= cellCount || position.y <= -1) {
        return true;
    }
    return false;
}

Game::Game(int WIDTH, int HEIGHT) {
	width = WIDTH;
	height = HEIGHT;
	InitWindow(2 * borderAllSides + width * cellSize,
               2 * borderAllSides + height * cellSize, "Snake Game");
    SetTargetFPS(60);
	
    InitAudioDevice();
    eatSound = LoadSound("Sound/eat.wav");
    wallSound = LoadSound("Sound/wall.wav");
    tailSound = LoadSound("Sound/tail.wav");
    music = LoadMusicStream("Music/Snake Game.wav");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.5);
}
Game::~Game() {
    UnloadSound(eatSound);
    UnloadSound(tailSound);
    UnloadSound(wallSound);
    CloseAudioDevice();
    CloseWindow();
}
void Game::Run() {
    while (WindowShouldClose() == false) {
        UpdateMusicStream(game.music);

        BeginDrawing();

        if (eventTriggered(0.5)) {
            game.Update();
        }

        int border = 4;
        DrawRectangleLinesEx(
            Rectangle{borderAllSides - border, borderAllSides - border,
                      cellCount * cellSize + 2 * border,
                      cellCount * cellSize + 2 * border},
            border, game.gameRunning ? WHITE : GRAY);
        int fontSize = 28;
        DrawText("Snake Game", borderAllSides - border, 4, fontSize,
                 game.gameRunning ? WHITE : GRAY);
        const char *scoreString = TextFormat("Score: %i", game.score);
        DrawText(scoreString, borderAllSides - border,
                 borderAllSides + border + (cellCount * cellSize), fontSize,
                 game.gameRunning ? WHITE : GRAY);

        if (game.gameRunning) {
            if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
                game.snake.direction = {0, -1};
            }
            if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
                game.snake.direction = {0, 1};
            }
            if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
                game.snake.direction = {-1, 0};
            }
            if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
                game.snake.direction = {1, 0};
            }
            ClearBackground(BLACK);
            game.Draw();
        } else {
            DrawText(
                "Game Over", borderAllSides + (cellCount * cellSize) / 2 - 80,
                borderAllSides + (cellCount * cellSize) / 2 - 32, 32, WHITE);
        }

        EndDrawing();
    }
}
void Game::Draw() {
    food.Draw(gameRunning);
    snake.Draw(gameRunning);
}
void Game::CheckCollisionWithEdges() {
    if (OutOfBound(snake.body[0])) {
        PlaySound(wallSound);
        GameOver();
    }
}
void Game::CheckCollisionWithTail() {
    deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();
    if (ElementInDeque(snake.body[0], headlessBody)) {
        PlaySound(tailSound);
        GameOver();
    }
}
void Game::GameOver() {
    // snake.Reset();
    // food.position = food.GenerateRandomPos(snake.body);
    snake.SetToLastPosition();
    cout << "Game Over!" << endl;
    gameRunning = false;
    // score = 0;
    StopMusicStream(music);
}
void Game::CheckCollisionWithFood() {
    if (Vector2Equals(snake.body[0], food.position)) {
        food.position = food.GenerateRandomPos(snake.body);
        snake.bodyGrows = true;
        PlaySound(eatSound);
        score++;
    }
}
void Game::Update() {
    if (gameRunning) {
        CheckCollisionWithEdges();
        CheckCollisionWithTail();
        snake.Update(gameRunning);
        CheckCollisionWithFood();
    }
}
