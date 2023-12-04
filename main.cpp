#include "Game.h"

#define N 20
#define HEIGHT N
#define WIDTH N

int main()
{
	Game game = Game(WIDTH, HEIGHT);
	game.Run();
	return 0;
}
