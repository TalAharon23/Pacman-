#include "LoadGameMode.h"

void LoadGameMode::runGame() {
	//Printing the menu and getting a choice from the user according to the menu options.
	system("cls");
	clearInput();

	init();

	setColorFlag(true);
	GameManager();
}

void LoadGameMode::GameManager() {

	GameStatus gs = GameStatus::NONE;

	//MULTI_GAMES
	for (int i = 0; (i < numOfScreens); i++) {
		currFileScreen = i;
		if (setBoardTemplate(filesNames[i])) {
			if (gs != GameStatus::WIN) { //If user lost --> dont continue to the next screen.
				scoreBoard.setLives(3);
				scoreBoard.setScore(0);
			}
			try {
				gs = playGame();
			}
			catch (const string& msg) {
				system("cls");
				gotoxy(5, 5);
				cout << msg << endl;
			}
			if (scoreBoard.getLives() <= 0)
				break;
			if (i != (numOfScreens - 1)) {
				system("cls");
				gotoxy(5, 5);
				cout << "Let's continue to the next board!" << endl;
				Sleep(3000);
			}
		}
	}

	setTextColor(Color::WHITE);
	system("cls");
	gotoxy(5, 5);
	cout << "Game load mode finished!! goodbye!" << endl;
}

GameStatus LoadGameMode::playGame() {
	//Handling with playing a game.
	system("cls");
	hideCursor();

	gameBoard.clearBreadCrumbsVector();
	gameBoard.setBreadCrumbsPoints();
	scoreBoard.setMaxScore(gameBoard.getNumOfBreadcrumbs());
	gameBoard.printBoard(colorFlag);
	scoreBoard.printScoreBoard(colorFlag);

	string stepsFileStr = filesNames[currFileScreen].substr(0, filesNames[currFileScreen].length() - 7);
	stepsFileStr = stepsFileStr.append(".steps");

	string resultFileStr = filesNames[currFileScreen].substr(0, filesNames[currFileScreen].length() - 7);
	resultFileStr = resultFileStr.append(".result");

	ifstream stepsFile(stepsFileStr, ios::in);
	ifstream resultFile(resultFileStr, ios::in);

	if (!stepsFile.good())
		throw "Invalid steps file!";
	if (!resultFile.good())
		throw "Invalid result file!";

	GameStatus game_status = GameStatus::NONE;
	bool flagGhosts = true;

	int timerCounter = 0;

	pac.draw();
	ghostsOperations(GHOST_DRAW, false, true, false);

	do {
		++timerCounter;

		if (isPauseGame(resultFile, timerCounter, false)) {
			game_status = GameStatus::ESCAPED;
			break;
		}

		readAndSetFromStepsFile(stepsFile, flagGhosts, false); //pacman

		if (isGameOver(game_status, (ofstream&)resultFile, timerCounter)) {
			Sleep(3000);
			break;
		}

		readGhostsSteps(stepsFile, flagGhosts, false);

		readAndSetFromStepsFile(stepsFile, flagGhosts, false); //fruit
		fruitEatenUtils();

		Sleep(80);

		if (isGameOver(game_status, (ofstream&)resultFile, timerCounter)) {
			Sleep(3000);
			break;
		}

	} while (true);

	stepsFile.close();
	resultFile.close();

	return game_status;
}
bool LoadGameMode::isFruitAppearanceText(ifstream& stepsFile) {
	int posLine = stepsFile.tellg();
	string line;
	getline(stepsFile, line);

	if (line[0] == 'F') {
		line = line.substr(2);
		if (line.starts_with("Appearance") || line.starts_with("Disappear")) {
			stepsFile.seekg(posLine);
			return true;
		}
	}
	stepsFile.seekg(posLine);
	return false;
}

bool LoadGameMode::isPauseGame(ifstream& resultFile, int timerCounter, bool isSilentMode) {
	int saveInd = resultFile.tellg();
	string line;
	getline(resultFile, line);
	if (line[0] == '4') {
		line = line.substr(32);
		if (stoi(line) == timerCounter) {
			resultFile.seekg(saveInd, ios::beg);
			return true;
		}
		else {
			resultFile.seekg(saveInd, ios::beg);
		}
	}
	if (isSilentMode)
		resultFile.seekg(saveInd, ios::beg);
	return false;
}

void LoadGameMode::readAndSetFromStepsFile(ifstream& stepsFile, bool& flagGhosts, bool isSilentMode) {
	string line;
	string dirStr;
	getline(stepsFile, line);
	char i;

	switch (line[0]) {
	case 'P':
		dirStr = line.substr(2);
		pac.setDirection(getDirectionFromString(dirStr));
		pac.move(gameBoard, scoreBoard, pac.getPos(), false, isSilentMode);

		break;
	case 'G':
		i = line[1];
		i = i - '0';
		dirStr = line.substr(3);

		if (i == 0)
			ghosts[0].setDirection(getDirectionFromString(dirStr));
		else if (i == 1)
			ghosts[1].setDirection(getDirectionFromString(dirStr));
		else if (i == 2)
			ghosts[2].setDirection(getDirectionFromString(dirStr));
		else
			ghosts[3].setDirection(getDirectionFromString(dirStr));

		ghosts[i].move(gameBoard, scoreBoard, pac.getPos(), true, isSilentMode);
		break;

	case 'F':
		i = line[2];
		dirStr = line.substr(2);
		if (dirStr.starts_with("Appearance")) {
			dirStr = dirStr.substr(11); //example: 0511 6
			int figureChar = dirStr[5];
			fruit.setFigure(figureChar - '0');
			dirStr.resize(dirStr.size() - 2);
			int num = stoi(dirStr);
			if (fruitAppearance) {
				if (!isSilentMode && (fruit.getPos().getX() != 0 && fruit.getPos().getY() != 0))
					fruit.erase(gameBoard, fruit.getPos());
			}
			else
				fruitAppearance = true;

			fruit.setPos({ num / 100, num % 100 });
			if (!isSilentMode)
				fruit.draw();
		}
		else if (dirStr.starts_with("Disappear")) {
			if (!isSilentMode) {
				if (fruitAppearance == true) {
					fruit.erase(gameBoard, fruit.getPos());
				}
			}
			fruitAppearance = false;
		}
		break;
	}
}

Directions LoadGameMode::getDirectionFromString(const string& str) {
	if (str.starts_with("UP"))
		return Directions::UP;
	else if (str.starts_with("DOWN"))
		return Directions::DOWN;
	else if (str.starts_with("LEFT"))
		return Directions::LEFT;
	else if (str.starts_with("RIGHT"))
		return Directions::RIGHT;
	else
		return Directions::STAY;
}

bool LoadGameMode::isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) {
	//Checking if there is a collision which caused to 0 lives, or if the user won (all food were eaten).
	if (isCollisionWithGhosts(true)) {
		Point saveCollisionPoint = pac.getPos();

		scoreBoard.decreaseLives();
		scoreBoard.updateLives(colorFlag);

		//Some "Dying effects" --> getting pac, and ghosts to their first place.
		Sleep(1500);
		pac.erase(gameBoard, pac.getPos());
		ghostsOperations(GHOST_ERASE, false, true, false);
		fruit.erase(gameBoard, fruit.getPos());

		gotoxy(saveCollisionPoint.getX(), saveCollisionPoint.getY());
		cout << gameBoard.getCharInBoard(saveCollisionPoint);

		setCreatures();
		pac.setDirection(Directions::STAY); // Pacman does'nt move.
	}

	if (scoreBoard.getLives() == 0) { //If lose
		game_status = GameStatus::LOSE;
		return true;
	}

	if (scoreBoard.getMaxScore() == 0) { //If win
		game_status = GameStatus::WIN;
		return true;
	}

	return false;
}

void LoadGameMode::fruitEatenUtils() {
	if (pac.getPos() == fruit.getPos()) {
		scoreBoard.setScore(scoreBoard.getScore() + ((int)fruit.getFigure()));
		scoreBoard.updateScore(colorFlag);
		fruit.erase(gameBoard, fruit.getPos());
		fruit.setPos(Point(0, 0));
		fruitAppearance = false;
	}
}

void LoadGameMode::readGhostsSteps(ifstream& stepsFile, bool& flagGhosts, bool isSilentMode) {
	//Handle the amount of read ghosts calls.
	//Each iteration moving the ghosts in even positions, and next iteration moving the ghosts in odd positions.
	if (numOfGhosts != 0)
	{
		if (numOfGhosts == 1)
			if (flagGhosts) {
				readAndSetFromStepsFile(stepsFile, flagGhosts, isSilentMode);
				flagGhosts = false;
			}
			else
				flagGhosts = true;
		else if (numOfGhosts == 2) {
			readAndSetFromStepsFile(stepsFile, flagGhosts, isSilentMode);
		}
		else if (numOfGhosts == 3)
			if (flagGhosts) {
				readAndSetFromStepsFile(stepsFile, flagGhosts, isSilentMode);
				readAndSetFromStepsFile(stepsFile, flagGhosts, isSilentMode);
				flagGhosts = false;
			}
			else
			{
				readAndSetFromStepsFile(stepsFile, flagGhosts, isSilentMode);
				flagGhosts = true;
			}
		else {
			readAndSetFromStepsFile(stepsFile, flagGhosts, isSilentMode);
			readAndSetFromStepsFile(stepsFile, flagGhosts, isSilentMode);
		}
	}

}