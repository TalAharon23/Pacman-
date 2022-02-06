#pragma once
#include "Game.h"

class SimpleGameMode : public Game {
	Menu menu;

public:
	SimpleGameMode() {}
	virtual void runGame() override;
	virtual void GameManager() override;
	virtual GameStatus playGame() override;
	virtual bool isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) override;
	bool isGameOverSimple(GameStatus& game_status);
};