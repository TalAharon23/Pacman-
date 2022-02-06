#pragma once
#include "Game.h"

class SaveGameMode : public Game {
	Menu menu;

public:
	SaveGameMode() {}

	virtual void runGame() override;
	virtual void GameManager() override;
	virtual GameStatus playGame() override;
	virtual bool isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) override;

	void writeAppearancesToFile(bool apperFlag, ofstream& stepsFile);
	void writeToStepsFile(ofstream& stepsFile, fileCreatures fc, Directions dir);
	void writeToResultFile(ofstream& resultFile, GameStatus status, int time);

};