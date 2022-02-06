#pragma once
#include "Creature.h"

class Pacman : public Creature
{

public:
	Pacman() : Creature({ 0, 0 }, { 0, 0 }, { 0,0 }, Directions::STAY, PAC, Color::WHITE) {};

	virtual void move(Board& board, ScoreBoard& scoreBoard, const Point& pacPos, bool isLoadMode, bool isSilentMode) override;
	virtual void erase(Board& board, Point erasePoint) override;
};