/*Board Class:
- Creates and manages the board.
- Have initializing board functions.
- Responsible for moving the game creatures (draw and erase).
*/

#pragma once

#include "Utils.h"
#include "Point.h"
#include "Directions.h"
#include <fstream>
#include <sstream>


class Board
{
public:
	enum fileErrors { INVALID_SB = 1, INVALID_PAC, INVALID_GHOST, VALID_BOARD };
	enum fileSymbols { FILE_WALL = '#', FOOD = '.', NONE = '%', LEGEND = '&', BOARD_PAC = '@', BOARD_GHOST = '$', BOARD_FRUIT = '^' };
	enum consoleRange { min_col_c = 0, max_col_c = 80, min_row_c = 0, max_row_c = 25 };
	enum Borders { UPPER_BORDER = 0, LEFT_BORDER, RIGHT_BORDER, BUTTOM_BORDER };
	int min_col = 0, max_col = 80, min_row = 0, max_row = 20; //Board range

private:
	char board[max_row_c][max_col_c];
	vector<Point> breadCrumbsPoints;

public:
	static constexpr char EMPTY = ' ', WALL = 219;

	Board() {}
	~Board() { clearBreadCrumbsVector(); }

	void clearBoard();
	bool readTemplateBoard(const string fileName);
	void getMaxRow();
	int setPointsOnBoard(Point& pSB, Point& pPacman, Point* pGhosts, int* numOfGhosts);
	void setNoneOnScoreBoard(Point SBPoint);
	void setBreadCrumbs();
	void setCharInBoard(Point p, char ch) { board[p.getY()][p.getX()] = ch; };
	void setBreadCrumbsPoints();

	const vector<Point>& getBreadCrumbsVector() { return breadCrumbsPoints; }
	void clearBreadCrumbsVector();

	void printBoard(bool isGameColored)const;
	bool isAFood(Point p) const;
	bool isHiddenDoor(Point nextPoint)const;
	bool setNextPosOnBoard(Point& nextPos);
	bool isAWall(Point p) const;
	int getNumOfBreadcrumbs() const;
	char getCharInBoard(Point p) const { return board[p.getY()][p.getX()]; };
};
