#include "SilentGameMode.h"

void SilentGameMode::GameManager() {
	bool flag = true;
	GameStatus gs = GameStatus::NONE;

	//MULTI_GAMES
	for (int i = 0; (i < numOfScreens) && flag; i++) {
		currFileScreen = i;
		if (setBoardTemplate(filesNames[i])) {
			try {
				gs = playGame();
			}
			catch (const string& msg) {
				system("cls");
				gotoxy(5, 5);
				cout << msg << endl;
			}
			if (gs == GameStatus::FAILED) //If user not win --> not continue to the next screen.
				flag = false;
		}
	}
	if (gs != GameStatus::FAILED)
		cout << "Test Passed Successfuly" << endl;
}

GameStatus SilentGameMode::playGame() {
	//Handling with playing a game.

	gameBoard.clearBreadCrumbsVector();
	gameBoard.setBreadCrumbsPoints();
	scoreBoard.setMaxScore(gameBoard.getNumOfBreadcrumbs());
	scoreBoard.setLives(3);

	string stepsFileStr = filesNames[currFileScreen].substr(0, filesNames[currFileScreen].length() - 7);
	stepsFileStr = stepsFileStr.append(".steps");

	string resultFileStr = filesNames[currFileScreen].substr(0, filesNames[currFileScreen].length() - 7);
	resultFileStr = resultFileStr.append(".result");

	ifstream stepsFile(stepsFileStr, ios::in);
	ifstream resultFile(resultFileStr, ios::in);

	resultFile._Seekbeg();

	if (!stepsFile.good())
		throw "Invalid steps file!";
	if (!resultFile.good())
		throw "Invalid result file!";

	GameStatus game_status = GameStatus::NONE;
	bool flagGhosts = true;
	bool runSuccessfuly = true;

	int timerCounter = 0;

	do {
		++timerCounter;

		readAndSetFromStepsFile(stepsFile, flagGhosts, true); //pacman

		if (isGameOver(game_status, (ofstream&)resultFile, timerCounter)) {
			break;
		}

		readGhostsSteps(stepsFile, flagGhosts, true);

		readAndSetFromStepsFile(stepsFile, flagGhosts, true); //fruit

		if (isGameOver(game_status, (ofstream&)resultFile, timerCounter)) {
			break;
		}

	} while (true);

	stepsFile.close();
	resultFile.close();

	return game_status;
}

bool SilentGameMode::isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) {
	return isGameOverSilent(game_status, (ifstream&)resultFile, timer);
}

bool SilentGameMode::isGameOverSilent(GameStatus& game_status, ifstream& resultFile, int timer) {
	//Checking if there is a collision which caused to 0 lives, or if the user won (all food were eaten).
	if (isCollisionWithGhosts(true)) {

		game_status = GameStatus::COLLIDE;

		if (!checkTestFileResult(resultFile, timer, game_status)) {
			game_status = GameStatus::FAILED;
			return true;
		}

		Point saveCollisionPoint = pac.getPos();

		scoreBoard.decreaseLives();

		setCreatures();
		pac.setDirection(Directions::STAY); // Pacman does'nt move.
	}

	if (isPauseGame(resultFile, timer, true)) {
		game_status = GameStatus::ESCAPED;
		if (!checkTestFileResult(resultFile, timer, GameStatus::ESCAPED)) {
			game_status = GameStatus::FAILED;
		}
		return true;
	}

	if (scoreBoard.getLives() == 0) {  //If lose
		game_status = GameStatus::LOSE;
		if (!checkTestFileResult(resultFile, timer, game_status))
			game_status = GameStatus::FAILED;
		return true;
	}

	if (scoreBoard.getMaxScore() == 0) { //If win
		game_status = GameStatus::WIN;
		if (!checkTestFileResult(resultFile, timer, game_status))
			game_status = GameStatus::FAILED;
		return true;
	}
	return false;
}

bool SilentGameMode::checkTestFileResult(ifstream& resultFile, int timer, GameStatus gs) {
	string line;
	getline(resultFile, line);
	char ch = line[0];
	int gsTime = 0;

	switch (ch) {
	case '1':
		if (gs != GameStatus::COLLIDE) {
			cout << "Test Failed! :: Expected Pacman to die!" << endl;
			return false;
		}
		line = line.substr(32);
		gsTime = stoi(line);
		if (gsTime != timer) {
			cout << "Test Failed! :: Not equal times on die!" << endl;
			return false;
		}
		break;
	case '2':
		if (gs != GameStatus::LOSE) {
			cout << "Test Failed! :: Expected Lose!" << endl;
			return false;
		}
		line = line.substr(31);
		gsTime = stoi(line);
		if (gsTime != timer) {
			cout << "Test Failed! :: Not equal times on lose!" << endl;
			return false;
		}
		break;

	case '3':
		if (gs != GameStatus::WIN) {
			cout << "Test Failed! :: Expected Win!" << endl;
			return false;
		}
		line = line.substr(46);
		gsTime = stoi(line);
		if (gsTime != timer) {
			cout << "Test Failed! :: Not equal times on win!" << endl;
			return false;
		}

		break;

	case '4':
		if (gs != GameStatus::ESCAPED) {
			cout << "Test Failed! :: Expected Escape!" << endl;
			return false;
		}
		line = line.substr(31);
		gsTime = stoi(line);
		if (gsTime != timer) {
			cout << "Test Failed! :: Not equal times on escape!" << endl;
			return false;
		}

		break;
	}

	return true;
}
