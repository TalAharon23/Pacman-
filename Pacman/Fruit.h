#pragma once
#include "Creature.h"

class Fruit : public Creature
{
public:
	Fruit() : Creature({ 0, 0 }, { 0, 0 }, { 0,0 }, Directions::LEFT, 0, Color::WHITE) {};

	void randPosition(const vector<Point>& breadCrumbsPoints);
	void randFigure();

	virtual void draw() override;

};