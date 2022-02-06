#pragma once
#include "Point.h"
#include "Directions.h"
#include "Board.h"
#include "ScoreBoard.h"

/// <summary>
/// Creature is a basic class object to Pacman, Ghost and Fruit.
/// </summary>

class Creature {

public:
	enum Level { NOVICE = 1, GOOD, SMART, PACMAN_LEVEL }; //Best, good and novice moves reffers to ghost and fruit.
	static constexpr char PAC = 2, GHOST = 234;

private:
	Point startingPos;
	Point pos;
	Point dirPos;
	Directions direction;
	char figure;
	Color color;

	Level level;
	int moveIntervals;

public:
	Creature(Point _startingPos, Point _pos, Point _dirPos, Directions _direction, char _figure, Color _color) :
		startingPos(_startingPos), pos(_pos), dirPos(_dirPos), direction(_direction), figure(_figure), color(_color),
		level(Creature::NOVICE), moveIntervals(0) {}

	//Setters:
	void setStartingPos(Point _startingPos) { startingPos.setPoint(_startingPos); }
	void setPos(Point _pos) { pos.setPoint(_pos); }
	void setDirPos(Point _dirPos) { dirPos.setPoint(_dirPos); }
	void setDirection(Directions dir) { direction = dir; }
	void setFigure(const char c) { figure = c; }
	void setColor(Color _color) { color = _color; }
	void setLevel(Level _level) { level = _level; }

	//--------Getters--------
	Point getStartingPos() const { return startingPos; }
	Point getPos() const { return pos; }
	Point getDirPos() const { return dirPos; }
	Directions getDirection()const { return direction; }
	char getFigure()const { return figure; }
	Color getColor()const { return color; }
	Level getLevel()const { return level; }

	//-----------------------
	void AddDirPosToPos();

	Directions getDirectionKey(char key, const char* arrowKeys);
	void changeDirection();

	void getDirectionGF(); // Getting randomly the ghost and fruit direction.

	virtual void draw();
	virtual void erase(Board& board, Point erasePoint);

	//---------Moves operations--------------
	virtual void move(Board& board, ScoreBoard& scoreBoard, const Point& pacPos, bool isLoadMode, bool isSilentMode);
	void moveStep(Board& board, bool isSilentMode);

	void getNextDir(Board& board, const Point& pacPos);
	bool isValidStep(const vector<vector<bool>> visit, int row, int col, Board& board);
	Point BFS(Board& board, vector<vector<bool>> visit, int row, int col);
};