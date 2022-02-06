#include "Menu.h"

void Menu::printMenu() const {
	//Printing the menu.
	cout << "-----------Pacman Game-----------" << endl;
	cout << "(1) Start a new game (no colors)" << endl;
	cout << "(2) Start a new game (with colors edition)" << endl;
	cout << "(8) Present instructions and keys" << endl;
	cout << "(9) Exit" << endl;
}

void Menu::printInstructions() const {
	//Printing the game instructions.
	system("cls");

	cout << "The player controls Pac-Man," << endl;
	cout << "who must eat all the dots inside an enclosed maze while avoiding two ghosts." << endl << endl;
	cout << "The game ends when you are eaten 3 times by the ghosts OR" << endl;
	cout << "if you have eaten all the dots." << endl << endl;
	cout << "The keys are:" << endl;
	cout << "w/W: UP" << endl;
	cout << "a/A: LEFT" << endl;
	cout << "d/D: RIGHT" << endl;
	cout << "x/X: DOWN" << endl;
	cout << "s/S: STAY" << endl;
	cout << "Press ESC to pause the game.\n\n" << endl;

	cout << "Press any key to get back to the menu" << endl;
	while (!_kbhit());
	//runGame();
}

int Menu::getGhostsLevels() const {
	system("cls");

	int choice;
	cout << "Enter the game's difficulty:" << endl;
	cout << "1. Easy" << endl;
	cout << "2. Normal" << endl;
	cout << "3. Hard" << endl;

	cin >> choice;
	while ((choice < 1) || (choice > 3)) {
		cout << "Invalid choice, try again." << endl;
		cin >> choice;
	}

	return choice;
}

int Menu::getGameMethod() const {
	system("cls");

	int choice;

	cout << "Press 1 to load a specific screen, or press 2 to play them all " << endl;

	cin >> choice;
	while ((choice < 1) || (choice > 2)) {
		cout << "Invalid choice, try again." << endl;
		cin >> choice;
	}

	//game_option = (gameOption)choice;
	return choice;
}

int Menu::getFileIndex(int numOfScreens, const vector<string> filesNames) const {
	system("cls");

	int choice;

	cout << "Choose a screen from the list below:" << endl;

	for (int i = 0; i < numOfScreens; i++)
		cout << i + 1 << ". " << filesNames[i] << endl;

	cin >> choice;
	while ((choice < 1) || (choice > numOfScreens)) {
		cout << "Invalid choice, try again." << endl;
		cin >> choice;
	}

	return choice;
}

void Menu::printLoseOrWin(GameStatus status, bool colorFlag, const ScoreBoard& scoreBoard) const {
	//Getting the game status and printing the relevant message to the user.
	system("cls");

	if (colorFlag)
		setTextColor(Color::BROWN);
	else
		setTextColor(Color::WHITE);

	gotoxy(5, 5);


	if (status == GameStatus::WIN) { //Win
		cout << "You are a winner!!";
		gotoxy(5, 6);
	}
	else if (status == GameStatus::LOSE) { //Lose
		cout << "You are a loser!";
		gotoxy(5, 6);
	}
	else { //Exit
		cout << "Thank you for playing";
		gotoxy(5, 6);
	}

	Sleep(1000);
	cout << "Your Score: " << scoreBoard.getScore();
	gotoxy(5, 7);

	if (status == GameStatus::LOSE || status == GameStatus::ESCAPED) {
		cout << "GOODBYE!" << endl;
		Sleep(3000);
	}
}

bool Menu::pauseGame(bool colorFlag) const {
	//Pausing the game and waiting for a key from the user about returning to the game/stop playing.
	system("cls");
	gotoxy(5, 5);

	if (colorFlag)
		setTextColor(Color::BROWN);
	else
		setTextColor(Color::WHITE);

	cout << "Game paused!";
	gotoxy(5, 5);

	cout << "Press ESC to get back to the game,";
	gotoxy(5, 6);
	cout << "or press any other key to return to the main menu.";

	gotoxy(5, 8);

	while (!_kbhit());

	if (_getch() == ESC) //If ESC entered --> return false --> return back to the game.
		return false;

	return true;
}