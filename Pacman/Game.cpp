#include "Game.h"

void Game::init() {

	if (!filesNames.empty()) { // it returns a true if the vector size is 0
		filesNames.clear();
		numOfScreens = 0;
	}

	initFilesNames();

	scoreBoard.setMaxScore(0);
	scoreBoard.setScore(0);
	scoreBoard.setLives(scoreBoard.max_lives);
	setColors();
}

void Game::fruitStep(int* countFruitSteps, int* countFruitDisSteps, int* countFruitPace) {
	int eatenFlag;
	(*countFruitDisSteps)--;

	eatenFlag = isFruitEaten();

	if (*countFruitDisSteps == 0) {
		fruitAppearance = true;
		summonFruit();
		fruit.draw();
		*countFruitSteps = rand() % 60 + 15;
	}

	//Checking for move to do:
	if (*countFruitSteps != 0 && eatenFlag == 3) { //If there is more steps to do and fruit have not eaten.
		if (*countFruitPace == 0) {
			fruit.move(gameBoard, scoreBoard, pac.getPos(), false, false);
			(*countFruitSteps)--;
			*countFruitPace = 5;
		}
		else
			(*countFruitPace)--;
	}

	if (eatenFlag == 3)
		eatenFlag = isFruitEaten();

	//If eaten or there is no more steps to do
	if (eatenFlag != 3 || (*countFruitSteps == 0 && *countFruitDisSteps < 0)) {

		fruitAppearance = false;
		fruit.erase(gameBoard, fruit.getPos());
		pac.draw();
		fruit.setPos({ 0,0 }); // For not eating fruit again. (To check!)
		*countFruitDisSteps = rand() % 40 + 40;
		*countFruitSteps = 0;

		if (eatenFlag == 1) { //fruit eaten by the pacman
			scoreBoard.setScore(scoreBoard.getScore() + ((int)fruit.getFigure()));
			scoreBoard.updateScore(colorFlag);
		}
	}
}

void Game::setCreatures() {
	//Setting all creatures/scoreBoard to starting positions.
	pac.setPos(pac.getStartingPos());
	scoreBoard.setPos(scoreBoard.getPos());

	for (int i = 0; i < numOfGhosts; i++) {
		ghosts[i].setPos(ghosts[i].getStartingPos());
	}
}

void Game::setGhostLevels(int level) {
	for (int i = 0; i < numOfGhosts; i++)
		ghosts[i].setLevel((Creature::Level)level);
}

void Game::summonFruit() {
	int posIndex;

	do {
		fruit.randPosition(gameBoard.getBreadCrumbsVector());
	} while (isACreaturePos(fruit.getPos()));

	fruit.randFigure();
}

bool Game::isACreaturePos(Point p) const {
	int row = p.getY(), col = p.getX();
	Point pacPos = pac.getPos();

	return ((pacPos == p) || isCollisionWithGhosts(false));
}


void Game::setColors() {
	if (isGameColored()) {
		pac.setColor(Color::YELLOW);
		fruit.setColor(Color::BROWN);
	}
	else {
		pac.setColor(Color::WHITE);
		fruit.setColor(Color::WHITE);
	}

	for (int i = 0; i < numOfGhosts; i++) {
		if (isGameColored())
			ghosts[i].setColor(Color::LIGHTRED);
		else
			ghosts[i].setColor(Color::WHITE);
	}
}

int Game::isFruitEaten() const {
	//Checking for fruit eaten.
	//Return 1 if eaten by the pacman, 2 if eaten by one of the ghosts, or 3 if not eaten.
	Point pacPos = pac.getPos();
	Point fruitPos = fruit.getPos();

	if (pacPos == fruitPos)
		return 1;

	if (isCollisionWithGhosts(false))
		return 2;

	return 3;
}

void Game::ghostsOperations(int oper, bool evenOrUnEvenMove, bool isLoadMode, bool isSilentMode) {
	int i;

	if (oper == GHOST_MOVE) {
		if (evenOrUnEvenMove) i = 0;
		else i = 1;

		for (i; i < numOfGhosts; i += 2) {
			ghosts[i].move(gameBoard, scoreBoard, pac.getPos(), isLoadMode, isSilentMode);
		}
	}
	else {
		for (int i = 0; i < numOfGhosts; i++) {
			if (oper == GHOST_DRAW && !isSilentMode)
				ghosts[i].draw();
			else if (oper == GHOST_ERASE && !isSilentMode)
				ghosts[i].erase(gameBoard, ghosts[i].getPos());
		}
	}
}

bool Game::isCollisionWithGhosts(bool flag) const {
	//If flag == true --> check collision between pacman and ghosts.
	//If flag == false --> check collision between fruit and ghosts.

	for (int i = 0; i < numOfGhosts; i++) {
		if (flag) {
			if (pac.getPos() == ghosts[i].getPos())
				return true;
		}
		else {
			if (fruit.getPos() == ghosts[i].getPos())
				return true;
		}
	}
	return false;
}

void Game::checkForGhostsCollision() {
	//Checking if there is a collision between two ghosts --> if there is making a novice move to seperate them.
	for (int i = 0; i < numOfGhosts - 1; i++) {
		for (int j = i + 1; j < numOfGhosts; j++) {
			if (ghosts[i].getPos() == ghosts[j].getPos()) {
				int level = ghosts[i].getLevel();
				ghosts[i].setLevel(Creature::NOVICE);
				ghosts[j].move(gameBoard, scoreBoard, pac.getPos(), false, false);
				ghosts[i].setLevel(Creature::Level(level));
			}
		}
	}
}

void Game::initFilesNames() {
	//Loading all screen's names from the working directory.
	std::string temp;

	for (const auto& file : std::filesystem::directory_iterator(".")) {
		if (file.path().string().ends_with(".screen")) {
			temp = file.path().string();
			temp.erase(0, 2);
			filesNames.push_back(temp);
			numOfScreens++;
		}
	}
	std::sort(filesNames.begin(), filesNames.end());
}

bool Game::setBoardTemplate(string fileName) {

	//Initialize board, scoreBoard and creatures.
	Point SBPos, pacPos;
	Point ghostsPoints[4];
	int _numOfGhosts;

	//if (!gameBoard.initBoard()) {
	if (!gameBoard.readTemplateBoard(fileName)) {
		system("cls");
		cout << "The file " << fileName << " is not valid!!";
		Sleep(3000);
		return false;
	}

	int flag = gameBoard.setPointsOnBoard(SBPos, pacPos, ghostsPoints, &_numOfGhosts);
	if (flag == gameBoard.INVALID_SB || flag == gameBoard.INVALID_PAC || flag == gameBoard.INVALID_GHOST) {
		system("cls");
		cout << "Creatures/Score board positions are not valid!!";
		Sleep(3000);
		return false;
	}

	pac.setStartingPos(pacPos);
	scoreBoard.setPos(SBPos);
	setNumOfGhosts(_numOfGhosts);
	fruit.setPos(Point(0, 0));

	gameBoard.setNoneOnScoreBoard(SBPos);

	for (int i = 0; i < _numOfGhosts; i++) {
		ghosts[i].setStartingPos(ghostsPoints[i]);
	}

	setCreatures();
	setColors();

	return true;
}