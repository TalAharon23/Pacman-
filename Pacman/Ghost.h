#pragma once
#include "Creature.h"

class Ghost : public Creature
{

public:
	Ghost(Point pos = { 0,0 }) : Creature({ 0, 0 }, pos, { 0,0 }, Directions::STAY, GHOST, Color::WHITE) {}
};
