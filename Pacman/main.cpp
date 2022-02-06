//#include "Game.h"
#include "SimpleGameMode.h"
#include "SaveGameMode.h"
#include "LoadGameMode.h"
#include "SilentGameMode.h"

void setGame(char* gameMode, char* silentModeString);

int main(int argc, char** argv)
{
	hideCursor();
	setGame(argv[1], argv[2]);
}

void setGame(char* gameMode, char* silentModeString) {
	Game* game = nullptr;
	if (strcmp(gameMode, "-simple") == 0)
		game = new SimpleGameMode();
	else if (strcmp(gameMode, "-load") == 0)
	{
		if (silentModeString != nullptr && strcmp(silentModeString, "-silent") == 0)
			game = new SilentGameMode();
		else
			game = new LoadGameMode();
	}
	else if ((strcmp(gameMode, "-save") == 0) && (silentModeString == nullptr) ||
		((strcmp(gameMode, "-save") == 0) && strcmp(silentModeString, "-silent") == 0)) {

		game = new SaveGameMode();
	}

	if (game != nullptr) {
		game->runGame();
		delete game;
	}
}