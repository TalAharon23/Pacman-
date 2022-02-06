#include "SaveGameMode.h"

void SaveGameMode::runGame() {
	//Printing the menu and getting a choice from the user according to the menu options.
	system("cls");
	clearInput();

	init();

	menu.printMenu();

	char choice;
	int screenMethod;
	int screenOption;
	bool flag = true;

	while (flag) {
		while (!_kbhit());
		choice = _getch();

		if (choice == NO_COLORS) {
			setColorFlag(false);
			GameManager();
			flag = false;
		}
		else if (choice == WITH_COLORS) { // with colors
			setColorFlag(true);
			GameManager();
			flag = false;
		}
		else if (choice == INSTRUCTIONS) {
			menu.printInstructions();
			runGame();
			flag = false;
		}
		else if (choice == ESCAPED_MENU) {
			system("cls");
			cout << "GOODBYE!" << endl;
			flag = false;
		}
	}
}

void SaveGameMode::GameManager() {
	int whichScreen;
	bool flag = true;

	setGameOption((gameOption)menu.getGameMethod());

	int level = menu.getGhostsLevels();

	if (game_option == SINGLE_GAME) {
		whichScreen = menu.getFileIndex(numOfScreens, filesNames);
		currFileScreen = whichScreen - 1;
		if (setBoardTemplate(filesNames[currFileScreen])) {
			setGhostLevels(level);
			playGame();
		}
	}
	else { // choice == MULTI_GAMES
		for (int i = 0; (i < numOfScreens) && flag; i++) {
			if (setBoardTemplate(filesNames[i])) {
				setGhostLevels(level);

				if (playGame() != GameStatus::WIN) //If user lost --> dont continue to the next screen.
					flag = false;
				else if (i != (numOfScreens - 1)) {
					system("cls");
					gotoxy(5, 5);
					cout << "Let's continue to the next board!" << endl;
					Sleep(3000);
				}
			}
		}
	}

	setTextColor(Color::WHITE);
	runGame();
}

GameStatus SaveGameMode::playGame() {
	//Handling the game.
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

	ofstream stepsFile(stepsFileStr, ios::out);
	ofstream resultFile(resultFileStr, ios::out);

	resultFile._Seekbeg();

	char key = 0;
	Directions dir;
	GameStatus game_status = GameStatus::NONE;

	int countFruitPace = 5;
	int countFruitSteps = 0;
	int countFruitDisSteps = 15;

	int timerCounter = 0;

	pac.draw();
	ghostsOperations(GHOST_DRAW, false, false, false);

	int tempEatenFlag;

	while (!_kbhit()); // Waiting for a direction to move pacman.
	bool flagGhosts = true, flagESC = false;

	do {
		++timerCounter;
		if (_kbhit())
		{
			key = _getch();

			if (key == ESC) { //Any other key that isn't ESC will finish the game.
				if (menu.pauseGame(colorFlag)) {
					game_status = GameStatus::ESCAPED;
					writeToResultFile(resultFile, game_status, timerCounter);
					menu.printLoseOrWin(game_status, colorFlag, scoreBoard);
					flagESC = false;
					break;
				}
				else {
					system("cls");
					gameBoard.printBoard(colorFlag);
					scoreBoard.printScoreBoard(colorFlag);
					flagESC = true;
				}
			}

			if ((dir = pac.getDirectionKey(key, arrowKeys)) != Directions::NOT_VALID)
				pac.setDirection(dir);
		}

		//Moving and checking for gameover twice (after each move -->move pacman and ghosts.)
		pac.move(gameBoard, scoreBoard, pac.getPos(), false, false);
		writeToStepsFile(stepsFile, fileCreatures::PAC, pac.getDirection());

		if (isGameOver(game_status, resultFile, timerCounter)) {
			Sleep(3000);
			break;
		}

		if (flagGhosts) { //Checking the flag and changing it --> moving ghosts after moving the pacman twice (half the pacman's speed).
			ghostsOperations(GHOST_MOVE, flagGhosts, false, false);
			writeToStepsFile(stepsFile, fileCreatures::G0, ghosts[0].getDirection());
			writeToStepsFile(stepsFile, fileCreatures::G2, ghosts[2].getDirection());

			flagGhosts = false;
		}
		else {
			ghostsOperations(GHOST_MOVE, flagGhosts, false, false);
			writeToStepsFile(stepsFile, fileCreatures::G1, ghosts[1].getDirection());
			writeToStepsFile(stepsFile, fileCreatures::G3, ghosts[3].getDirection());
			flagGhosts = true;
		}

		//checkForGhostsCollision();

		writeAppearancesToFile(fruitAppearance, stepsFile);
		fruitStep(&countFruitSteps, &countFruitDisSteps, &countFruitPace);

		Sleep(80);

		if (isGameOver(game_status, resultFile, timerCounter)) {
			Sleep(3000);
			break;
		}

	} while (key != ESC || flagESC);


	if (game_status != GameStatus::WIN) {
		cout << "\n\nPress any key to get back to the menu" << endl;
		while (!_kbhit());
	}

	stepsFile.close();
	resultFile.close();


	return game_status;
}

void SaveGameMode::writeAppearancesToFile(bool appearFlag, ofstream& stepsFile) {
	int fX = fruit.getPos().getX();
	int fY = fruit.getPos().getY();

	string x = std::to_string(fX), y = std::to_string(fY);

	if (fX < 10) {
		x[0] = '0';
		x[1] = fX + '0';
	}

	if (fY < 10) {
		y[0] = '0';
		y[1] = fY + '0';
	}

	if (appearFlag) // print Appearance 
		stepsFile << fileCreaturesCh[(int)fileCreatures::FRUIT] << " Appearance " << x[0] << x[1] << y[0] << y[1] << ' ' << (int)fruit.getFigure() << endl;
	else
		stepsFile << fileCreaturesCh[(int)fileCreatures::FRUIT] << " Disappear " << endl;
}

bool SaveGameMode::isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) {
	//Checking if there is a collision which caused to 0 lives, or if the user won (all crumbs were eaten).
	if (isCollisionWithGhosts(true)) {
		Point saveCollisionPoint = pac.getPos();

		writeToResultFile(resultFile, GameStatus::COLLIDE, timer);
		scoreBoard.decreaseLives();
		scoreBoard.updateLives(colorFlag);

		//Some "Dying effects" --> getting pac, and ghosts to their first place.
		Sleep(1500);
		pac.erase(gameBoard, pac.getPos());
		ghostsOperations(GHOST_ERASE, false, false, false);
		fruit.erase(gameBoard, fruit.getPos());

		gotoxy(saveCollisionPoint.getX(), saveCollisionPoint.getY());
		cout << gameBoard.getCharInBoard(saveCollisionPoint);

		setCreatures();
		pac.setDirection(Directions::STAY); // Pacman does'nt move.
	}

	if (scoreBoard.getLives() == 0) { //If lose
		game_status = GameStatus::LOSE;
		writeToResultFile(resultFile, game_status, timer);
		menu.printLoseOrWin(game_status, colorFlag, scoreBoard);
		return true;
	}

	//if (scoreBoard.getScore() == scoreBoard.getMaxScore()) { //If win
	if (scoreBoard.getMaxScore() == 0) { //If win
		game_status = GameStatus::WIN;
		writeToResultFile(resultFile, GameStatus::WIN, timer);
		menu.printLoseOrWin(game_status, colorFlag, scoreBoard);
		return true;
	}

	return false;
}

void SaveGameMode::writeToStepsFile(ofstream& stepsFile, fileCreatures fc, Directions dir) {
	if (fc <= 3 && fc >= numOfGhosts) //is a ghost and is not in the game.
		return;

	stepsFile << fileCreaturesCh[(int)(fc)] << " " << directions[(int)dir] << endl;
}

void SaveGameMode::writeToResultFile(ofstream& resultFile, GameStatus status, int time) {
	if (status == GameStatus::COLLIDE)
		resultFile << "1.Pacman died at point of time: " << time << endl;
	else if (status == GameStatus::LOSE)
		resultFile << "2.Game ended at point of time: " << time << endl;
	else if (status == GameStatus::WIN)
		resultFile << "3.Game finished successfuly at point of time: " << time << endl;
	else if (status == GameStatus::ESCAPED)
		resultFile << "4.Game closed at point of time: " << time << endl;
}
