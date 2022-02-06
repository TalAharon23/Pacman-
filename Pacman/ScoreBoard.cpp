#include "ScoreBoard.h"

void ScoreBoard::printScoreBoard(bool isGameColored) {

	if (isGameColored)
		setTextColor(Color::BROWN);
	else setTextColor(Color::WHITE);

	if (!isGameColored)
		setTextColor(Color::WHITE);
	else
		setTextColor(Color::YELLOW);

	gotoxy(pos.getX() + 4, pos.getY());
	cout << "PACMAN  GAME";

	//Lives and Score printing.
	setPrintingColor(isGameColored, false);
	gotoxy(pos.getX() + 5, pos.getY() + 1);
	cout << "Lives: " << lives << "/3";

	setPrintingColor(isGameColored, true);

	gotoxy(pos.getX() + 5, pos.getY() + 2);
	cout << "Score: " << score;
}

void ScoreBoard::updateScore(bool isGameColored) {
	setPrintingColor(isGameColored, true);
	gotoxy(pos.getX() + 5, pos.getY() + 2);
	cout << "Score: " << score;
}

void ScoreBoard::updateLives(bool isGameColored) {
	setPrintingColor(isGameColored, false);
	gotoxy(pos.getX() + 5, pos.getY() + 1);
	cout << "Lives: " << lives << "/3";
}


void ScoreBoard::setPrintingColor(bool isGameColored, bool whichPrint) {
	//Getting a flag(to set text with color or not-white) and another boolean for setting the score color.

	if (isGameColored) {
		if (whichPrint)  // Score printing
			setTextColor(Color::BROWN);
		else // Lives printing
			setTextColor(Color::RED);
	}
	else
		setTextColor(Color::WHITE);
}

bool ScoreBoard::isPointInFrame(Point p) const {
	if (((p.getX() >= pos.getX()) && (p.getX() <= (pos.getX() + 19)))
		&& ((p.getY() >= pos.getY()) && (p.getY() <= (pos.getY() + 2))))
		return true;

	return false;
}