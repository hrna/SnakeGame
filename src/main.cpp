#include "headers/SnakeGame.h"
#include <cstring>


int main(int argc, char* argv[]) {
	SnakeGame game;

	char *a {argv[1]};
	std::string arg1 {};
	
	if (a != NULL)
	{
		for (size_t i = 0; i < strlen(a); i++)
			arg1 += a[i];

		if (arg1 == "--mode=auto")
		{
			//Autopilot on
			game.play(true);
		}
		else
			game.play(false);
	}
	else
		game.play(false);

	return 0;
}
