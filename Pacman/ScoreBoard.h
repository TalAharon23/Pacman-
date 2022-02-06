/*ScoreBoard Class:
- Manages game's score board.
- Setting and printing the lives and score window.
- Holds and changing the lives and score attributes
*/

#pragma once
#include "Utils.h"
#include "Point.h"

class ScoreBoard
{
	const enum { min_y = 20, max_y = 24, min_x = 30, max_x = 50 }; //Window scales. // 0 19
	int max_score;
	int score;
	int lives;
	Point pos;

public:
	const int max_lives = 3;

	ScoreBoard() : max_score(0), score(0), lives(3) {}

	void setMaxScore(int _maxScore) { max_score = _maxScore; };
	void setScore(int _score) { score = _score; };
	void setLives(int _lives) { lives = _lives; }
	void setPos(Point _pos) { pos.setPoint(_pos); }
	void addScore() { score++; };
	void decreaseLives() { lives--; };
	void decreaseMaxScore() { max_score--; };

	int getLives()const { return lives; }
	int getScore()const { return score; }
	int getMaxScore()const { return max_score; }
	Point getPos() const { return pos; }

	void printScoreBoard(bool isGameColored);
	void setPrintingColor(bool isGameColored, bool whichPrint);
	void updateLives(bool isGameColored);
	void updateScore(bool isGameColored);
	bool isPointInFrame(Point p) const;
};
