#include "Board.h"

void Board::clearBoard() {
	for (int i = min_row_c; i < max_row_c; i++) {
		for (int j = min_col_c; j < max_col_c; j++) {
			board[i][j] = NONE;
		}
	}
}

bool Board::readTemplateBoard(const string fileName) {
	clearBoard();
	ifstream boardFile(fileName, ios::in);

	int i = 0, j = 0;
	char temp;
	bool fEOF = false;
	boardFile.seekg(0);

	temp = boardFile.get();

	while (temp != EOF) {

		if ((temp == '\n') || j == 80) { //End of Line
			if (i == 0 && j == 0) return false; // If first row is Empty --> not valid board!
			if (i == 0) max_col = j;
			i++;
			j = 0;
		}
		else {
			if (temp == FILE_WALL)
				temp = WALL;
			board[i][j++] = temp;
		}

		temp = boardFile.get();
	}

	boardFile.close();

	max_row = i;
	setBreadCrumbs();

	return true;
}

void Board::getMaxRow() {
	//Checking for the last row.
	for (int i = max_row_c; i > min_row_c; i--) {
		for (int j = max_col_c; j > min_col_c; j--) {
			if (board[i][j] == '\n' || board[0][0] == '\0') {
				max_row = i;
				return;
			}
		}
	}
}

void Board::setNoneOnScoreBoard(Point SBPoint) {
	//Checking for the last row.
	for (int i = SBPoint.getY(); i < SBPoint.getY() + 3; i++) {
		for (int j = SBPoint.getX(); j < SBPoint.getX() + 20; j++) {
			board[i][j] = NONE;
		}
	}
}

void Board::setBreadCrumbs() {
	//Putting breadcrumbs on every empty place in board
	for (int i = min_row; i <= max_row; i++) {
		for (int j = min_col; j <= max_col; j++) {
			if (board[i][j] == EMPTY)
				board[i][j] = FOOD;
		}
	}
}

int Board::setPointsOnBoard(Point& pSB, Point& pPacman, Point* pGhosts, int* numOfGhosts) {
	int countSB = 0, countPacman = 0, countGhosts = 0;

	for (int i = min_row; i <= max_row; i++) {
		for (int j = min_col; j <= max_col; j++) {
			//printBoard(true);
			if (board[i][j] == LEGEND) {
				countSB++;
				pSB.setPoint({ j, i });
				board[i][j] = EMPTY;
			}
			if (board[i][j] == BOARD_PAC) {
				countPacman++;
				pPacman.setPoint({ j, i });
				board[i][j] = EMPTY;
			}
			if (board[i][j] == BOARD_GHOST) {
				pGhosts[countGhosts].setPoint({ j, i });
				countGhosts++;
				board[i][j] = EMPTY;
			}
		}
	}

	if (countSB != 1)
		return INVALID_SB;
	if (countPacman != 1)
		return INVALID_PAC;
	if (countGhosts > 4)
		return INVALID_GHOST;

	*numOfGhosts = countGhosts;
	return VALID_BOARD;
}

void Board::setBreadCrumbsPoints() {
	for (int i = min_row; i < max_row; i++) {
		for (int j = min_col; j < max_col; j++) {
			if (board[i][j] == FOOD)
				breadCrumbsPoints.push_back({ j,i });
		}
	}
}

void Board::clearBreadCrumbsVector() {
	if (!breadCrumbsPoints.empty())
		breadCrumbsPoints.clear();
}

void Board::printBoard(bool isGameColored) const {
	//Printing the board.
	Point p;
	for (int i = min_row; i < max_row; i++) {
		for (int j = min_col; j < max_col; j++) {
			p.setX(j);
			p.setY(i);
			if (board[i][j] == NONE) cout << EMPTY;
			else {
				if (isGameColored == true) {
					if (!isAWall(p))
						setTextColor(Color::BLUE);
					else if (isAFood(p))
						setTextColor(Color::GREEN);

					cout << board[i][j];
				}
				else {
					setTextColor(Color::WHITE);
					cout << board[i][j];
				}
			}
		}
		cout << endl;
	}
}

bool Board::isAFood(Point p)const {
	//Returns true if the position on board is a food.
	if (board[p.getY()][p.getX()] == FOOD) return true;

	return false;
}

bool Board::isHiddenDoor(Point nextPoint) const {
	int row = nextPoint.getY();
	int col = nextPoint.getX();

	if (board[row][col] == EMPTY || board[row][col] == FOOD)
		return true;

	return false;
}

bool Board::setNextPosOnBoard(Point& nextPos) {
	//Checking if the next position to be is out of frame --> if it does,
	//changing it to the opposite side of board, and return true.

	int row = nextPos.getY();
	int col = nextPos.getX();

	if (row < min_row) {
		nextPos.setPoint({ col, max_row - 1 });
		return true;
	}
	if (row > max_row - 1) {
		nextPos.setPoint({ col, min_row });
		return true;
	}
	if (col < min_col) {
		nextPos.setPoint({ max_col - 1, row });
		return true;
	}
	if (col > max_col - 1) {
		nextPos.setPoint({ min_col, row });
		return true;
	}

	return false;
}

bool Board::isAWall(Point p) const {
	//Returns true if the position on board is a wall.
	if (board[p.getY()][p.getX()] == WALL)
		return false;
	return true;
}

int Board::getNumOfBreadcrumbs() const {
	int count = 0;

	for (int i = min_row; i <= max_row; i++) {
		for (int j = min_col; j <= max_col; j++) {
			if (board[i][j] == FOOD)
				count++;
		}
	}
	return count;
}
