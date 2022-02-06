#pragma once
#include "Utils.h"
#include "ScoreBoard.h"
#include "GameStatus.h"

class Menu
{
public:
	const int ESC = 27;

	void printMenu()const;
	void printInstructions() const;
	void printLoseOrWin(GameStatus status, bool colorFlag, const ScoreBoard& scoreBoard)const;
	int getGhostsLevels()const;
	int getGameMethod()const;
	int getFileIndex(int numOfScreens, const vector<string> filesNames) const;
	bool pauseGame(bool colorFlag)const;
};