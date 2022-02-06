#include "Creature.h"

void Creature::draw() {
	gotoxy(pos.getX(), pos.getY());
	setTextColor(color);

	cout << figure << endl;
}

Directions Creature::getDirectionKey(char key, const char* arrowKeys) {
	//Getting an entered key and returns the direction number of that key.
	for (int i = 0; i < 5; i++)
	{
		if (key == arrowKeys[i] || tolower(key) == arrowKeys[i])
			return (Directions)i;
	}
	return Directions::NOT_VALID;
}


void Creature::changeDirection() { //Changing direction, according to his direction number.
	Directions dir = getDirection();
	switch (dir)
	{
	case Directions::LEFT: //Left - a
		setDirPos({ -1, 0 });
		break;
	case Directions::RIGHT: // RIGHT - d
		setDirPos({ 1, 0 });
		break;
	case Directions::UP: // UP - w
		setDirPos({ 0, -1 });
		break;
	case Directions::DOWN: // DOWN - x
		setDirPos({ 0, 1 });
		break;
	case Directions::STAY: // STAY - s
		setDirPos({ 0, 0 });
		break;
	}
}

void Creature::getDirectionGF() {
	//Setting a random direction to the ghost and the fruit.
	Directions dir = (Directions)(rand() % 4);
	setDirection(dir);
}

void Creature::AddDirPosToPos() {
	pos.setX(pos.getX() + dirPos.getX());
	pos.setY(pos.getY() + dirPos.getY());
}

//-------------------------------------

void Creature::erase(Board& board, Point erasePoint) {

	int x = erasePoint.getX();
	int y = erasePoint.getY();

	gotoxy(x, y);

	if (color == Color::WHITE)
		setTextColor(Color::WHITE);
	else
		setTextColor(Color::GREEN);

	if (board.getCharInBoard(erasePoint) != Board::NONE && board.getCharInBoard(erasePoint) != Board::WALL)
		cout << board.getCharInBoard(erasePoint) << endl;
}

//--------------Moves Functions-----------------------
void Creature::move(Board& board, ScoreBoard& scoreBoard, const Point& pacPos, bool isLoadMode, bool isSilentMode) {

	switch (level)
	{
	case Creature::SMART:
		getNextDir(board, pacPos);
		moveStep(board, false);
		break;
	case Creature::GOOD:
		if (moveIntervals >= 12) { //If already 12 smart moves made.
			if (moveIntervals < 20) { //If num of moves between 12 to 20 --> do novice move (8 novice moves).
				moveIntervals++;
				getDirectionGF();
				moveStep(board, false);
			}
			else { //Reset moves intervals, and do smart move.
				moveIntervals = 0;
				getNextDir(board, pacPos);
				moveStep(board, false);
			}
		}
		else { //Making 15 smart moves.
			moveIntervals++;
			getNextDir(board, pacPos);
			moveStep(board, false);
		}
		break;
	case Creature::NOVICE: //Fruit move
		if (!isLoadMode)
			getDirectionGF();

		moveStep(board, isSilentMode);
		break;
	default:
		break;
	}
}

void Creature::moveStep(Board& board, bool isSilentMode) {
	//Novice move

	changeDirection();

	Point nextPos, tempPos2 = getPos();
	int xPlusDir = getPos().getX() + getDirPos().getX(), yPlusDir = getPos().getY() + getDirPos().getY();

	nextPos.setX(xPlusDir);
	nextPos.setY(yPlusDir);

	if (board.setNextPosOnBoard(nextPos))
		return;

	if (!board.isAWall(nextPos)) //If next step is a wall.
		return;

	AddDirPosToPos();

	if (!isSilentMode) {
		draw();
		erase(board, tempPos2);
	}

}

void Creature::getNextDir(Board& board, const Point& pacPos) {
	int xPacPos = pacPos.getX(), yPacPOs = pacPos.getY();
	int xGhostPos = pos.getX(), yGhostPos = pos.getY();

	vector<vector<bool>> visit(board.max_row + 1, vector<bool>(board.max_col + 1, false));

	Point newPos = BFS(board, visit, yPacPOs, xPacPos);

	//newPos is to the left
	if (newPos.getX() == xGhostPos - 1) setDirection(Directions::LEFT);

	//newPos is to the right
	else if (newPos.getX() == xGhostPos + 1) setDirection(Directions::RIGHT);

	//newPos is to the up
	else if (newPos.getY() == yGhostPos - 1) setDirection(Directions::UP);

	//newPos is to the down
	else setDirection(Directions::DOWN);
}

bool Creature::isValidStep(const vector<vector<bool>> visit, int row, int col, Board& board) {
	// If cell is bounds
	if (row >= board.max_row || col >= board.max_col)
		return false;

	// If cell is bounds
	if (row <= 0 || col <= 0 || !board.isAWall({ col, row }) /*|| board.GetCoordinate(col, row) == TUNNEL*/)
		return false;

	// If cell is already visited
	if (visit[row][col])
		return false;

	// Otherwise
	return true;
}


Point Creature::BFS(Board& board, vector<vector<bool>> visit, int row, int col) {
	int test = 0;
	Point ghostPos = getPos();

	//U, R, D, L
	int dRow[] = { -1, 0, 1, 0 };
	int dCol[] = { 0, 1, 0, -1 };

	// Stores indices of the matrix cells
	queue<Point> q;
	int xGhost = getPos().getX();
	int yGhost = getPos().getY();
	// Mark the starting cell as visited and push it into the queue
	q.push(Point{ col, row });
	visit[row][col] = true;

	// Iterate while the queue is not empty
	while (!q.empty()) {

		Point cell = q.front();
		int x = cell.getX();
		int y = cell.getY();

		q.pop();

		// Go to the adjacent cells
		for (int i = 0; i < 4; i++) {
			int adjy = y + dRow[i];
			int adjx = x + dCol[i];
			if (Point(adjx, adjy) == ghostPos)
				return Point(x, y);
			else if (isValidStep(visit, adjy, adjx, board)) {
				q.push({ adjx, adjy });
				visit[adjy][adjx] = true;
			}
		}
	}

	// if short way has not found - return the curr position

	return getPos();
}