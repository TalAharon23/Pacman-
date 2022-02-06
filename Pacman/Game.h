/*Game Class:
- Manages game board, creatures and score board and Menu printing and getting functions.
- Operates the game. checks and prints game's status.
*/

#pragma once
#include "Menu.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Fruit.h"
#include "string"

namespace fs = std::filesystem;

class Game
{
public:
	enum menuOptions { NO_COLORS = '1', WITH_COLORS = '2', INSTRUCTIONS = '8', ESCAPED_MENU = '9' };
	enum gameModes { LOAD_MODE = 1, SILENT_LOAD_MODE, SAVE_MODE };
	enum gameOption { SINGLE_GAME = 1, MULTI_GAMES };
	enum ghostOperations { GHOST_MOVE = 1, GHOST_DRAW, GHOST_ERASE, GHOST_SET_LEVEL };

	enum fileCreatures { G0 = 0, G1, G2, G3, PAC, FRUIT };
	const string fileCreaturesCh[6] = { "G0", "G1", "G2", "G3", "P", "F" };
	const string directions[5] = { "LEFT" , "RIGHT", "UP", "DOWN", "STAY" };
	const string fruitUtils[2] = { "Appearance", "Dissappearance" };

	const int ESC = 27;
	constexpr static char arrowKeys[6] = "adwxs";

protected:

	Pacman pac;
	Ghost ghosts[4];
	Fruit fruit;
	Board gameBoard;
	ScoreBoard scoreBoard;
	//Menu menu;

	int numOfGhosts;
	bool colorFlag;

	int numOfScreens;
	int currFileScreen;
	vector<string> filesNames;

	GameStatus game_status = GameStatus::NONE;
	gameOption game_option;

	bool fruitAppearance;

public:
	Game() : numOfGhosts(0), colorFlag(false), numOfScreens(0), currFileScreen(0), game_option(Game::gameOption::MULTI_GAMES), fruitAppearance(false) {};
	void init();
	virtual void runGame() = 0; //לממש אצל הנורשים
	virtual void GameManager() = 0;
	virtual GameStatus playGame() = 0;
	virtual bool isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) = 0;

	void setCreatures();
	void setGhostLevels(int level);
	void setColors();

	void fruitStep(int* countFruitSteps, int* countFruitDisSteps, int* countFruitPace);
	void summonFruit();
	bool isACreaturePos(Point p) const;
	int isFruitEaten() const;

	void setColorFlag(bool color) { colorFlag = color; }
	void setNumOfGhosts(int num) { numOfGhosts = num; }
	bool isGameColored() { return colorFlag; }
	gameOption getGameOption() const { return game_option; }
	void setGameOption(gameOption go) { game_option = go; }

	void ghostsOperations(int oper, bool evenOrUnEvenMove, bool isLoadMode, bool isSilentMode);
	bool isCollisionWithGhosts(bool flag)const;
	void checkForGhostsCollision();

	void initFilesNames();
	bool setBoardTemplate(string fileName);
};



