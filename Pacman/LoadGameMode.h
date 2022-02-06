#pragma once
#include "Game.h"

class LoadGameMode : public Game {

public:
	virtual void runGame() override;
	virtual void GameManager() override;
	virtual GameStatus playGame() override;
	virtual bool isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) override;


	bool isFruitAppearanceText(ifstream& stepsFile);
	bool isPauseGame(ifstream& resultFile, int timerCounter, bool isSilentMode);
	void readAndSetFromStepsFile(ifstream& stepsFile, bool& flagGhosts, bool isSilentMode);
	Directions getDirectionFromString(const string& str);
	void fruitEatenUtils();
	void readGhostsSteps(ifstream& stepsFile, bool& flagGhosts, bool isSilentMode);
};