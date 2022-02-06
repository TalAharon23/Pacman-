#pragma once
#include "LoadGameMode.h"

class SilentGameMode : public LoadGameMode {

public:
	virtual void GameManager() override;
	virtual GameStatus playGame() override;
	virtual bool isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) override;
	bool isGameOverSilent(GameStatus& game_status, ifstream& resultFile, int timer);
	bool checkTestFileResult(ifstream& resultFile, int timer, GameStatus gs);
};