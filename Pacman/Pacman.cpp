#include "Pacman.h"

void Pacman::move(Board& board, ScoreBoard& scoreBoard, const Point& pacPos, bool isLoadMode, bool isSilentMode) {
	changeDirection();

	Point nextPos, tempPos2 = getPos();
	int xPlusDir = getPos().getX() + getDirPos().getX(), yPlusDir = getPos().getY() + getDirPos().getY();

	nextPos.setX(xPlusDir);
	nextPos.setY(yPlusDir);

	board.setNextPosOnBoard(nextPos);

	if (board.isHiddenDoor(nextPos)) {//If next step is hidden door. 
		erase(board, getPos());
		setPos(nextPos);
	}
	else if (!board.isAWall(nextPos)) {//If next step is a wall.
		setDirection(Directions::STAY);
		return;
	}
	else AddDirPosToPos();

	if (!isSilentMode)
		draw();

	if (getDirection() != Directions::STAY) {
		if (board.isAFood(getPos())) { //If the last position was a food - before erasing - updating score and delete food from board.
			scoreBoard.addScore();
			scoreBoard.decreaseMaxScore();
			if (!isSilentMode)
				scoreBoard.updateScore(getColor() == Color::WHITE ? false : true);
			board.setCharInBoard(getPos(), Board::EMPTY);
		}
		if (!isSilentMode)
			erase(board, tempPos2);
	}
}

void Pacman::erase(Board& board, Point erasePoint) {
	//Getting the board and boolean flag if the erase is on ghost or fruit --> if it does --> return the existing char in board.
	int x = erasePoint.getX();
	int y = erasePoint.getY();

	gotoxy(x, y);

	if (getColor() == Color::WHITE)
		setTextColor(Color::WHITE);
	else
		setTextColor(Color::GREEN);

	cout << Board::EMPTY << endl;
}
