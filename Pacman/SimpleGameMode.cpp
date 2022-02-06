#include "SimpleGameMode.h"

void SimpleGameMode::runGame() {
	//Printing the menu and getting a choice from the user according to the menu options.
	system("cls");
	clearInput();

	init();

	menu.printMenu();

	char choice;
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

void SimpleGameMode::GameManager() {
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

				if (playGame() != GameStatus::WIN) //If user not win --> not continue to the next screen.
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

GameStatus SimpleGameMode::playGame() {
	//Handling with playing a game.
	system("cls");
	hideCursor();

	gameBoard.clearBreadCrumbsVector();
	gameBoard.setBreadCrumbsPoints();
	scoreBoard.setMaxScore(gameBoard.getNumOfBreadcrumbs());
	gameBoard.printBoard(colorFlag);
	scoreBoard.printScoreBoard(colorFlag);

	char key = 0;
	Directions dir;
	GameStatus game_status = GameStatus::NONE;

	int countFruitPace = 5;
	int countFruitSteps = 0;
	int countFruitDisSteps = 15;

	pac.draw();
	ghostsOperations(GHOST_DRAW, false, false, false);

	while (!_kbhit()); // Waiting for a direction to move pacman.
	bool flagGhosts = true, flagESC = false;

	do {
		if (_kbhit())
		{
			key = _getch();

			if (key == ESC) {
				if (menu.pauseGame(colorFlag)) {
					game_status = GameStatus::ESCAPED;
					menu.printLoseOrWin(game_status, colorFlag, scoreBoard);
					flagESC = false;
					break;
				}
				else { //Any other key that isn't ESC will finish the game.
					system("cls");
					gameBoard.printBoard(colorFlag);
					scoreBoard.printScoreBoard(colorFlag);
					flagESC = true;
				}
			}

			if ((dir = pac.getDirectionKey(key, arrowKeys)) != Directions::NOT_VALID)
				pac.setDirection(dir);
		}

		//Moving and checking for game over 2 times (after each move --> pacman and ghosts move).
		pac.move(gameBoard, scoreBoard, pac.getPos(), false, false);

		if (isGameOverSimple(game_status)) {
			Sleep(3000);
			break;
		}

		if (flagGhosts) { //Checking the flag and changing it --> moving ghosts after moving the pacman two times (half from the pacman speed).
			ghostsOperations(GHOST_MOVE, flagGhosts, false, false);
			flagGhosts = false;
		}
		else {
			ghostsOperations(GHOST_MOVE, flagGhosts, false, false);
			flagGhosts = true;
		}

		checkForGhostsCollision();

		fruitStep(&countFruitSteps, &countFruitDisSteps, &countFruitPace);

		Sleep(80);

		if (isGameOverSimple(game_status)) {
			Sleep(3000);
			break;
		}

	} while (key != ESC || flagESC);


	if (game_status != GameStatus::WIN) {
		cout << "\n\nPress any key to get back to the menu" << endl;
		while (!_kbhit());
	}

	return game_status;
}

bool SimpleGameMode::isGameOver(GameStatus& game_status, ofstream& resultFile, int timer) {
	return isGameOverSimple(game_status);
}

bool SimpleGameMode::isGameOverSimple(GameStatus& game_status) {
	//Checking if there is a collision which caused to 0 lives, or if the user won (all food were eaten).
	if (isCollisionWithGhosts(true)) {
		Point saveCollisionPoint = pac.getPos();

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
		menu.printLoseOrWin(game_status, colorFlag, scoreBoard);
		return true;
	}

	if (scoreBoard.getMaxScore() == 0) { //If win
		game_status = GameStatus::WIN;
		menu.printLoseOrWin(game_status, colorFlag, scoreBoard);
		return true;
	}

	return false;
}