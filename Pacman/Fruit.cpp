#include "Fruit.h"

void Fruit::randPosition(const vector<Point>& breadCrumbsPoints) {
	srand(time(NULL));

	int randInd = rand() % (breadCrumbsPoints.size());

	setPos(breadCrumbsPoints[randInd]);
}

void Fruit::randFigure() {
	setFigure(rand() % 5 + 5);
}

void Fruit::draw() {
	if (getPos().getX() != 0 && getPos().getY() != 0) {
		gotoxy(getPos().getX(), getPos().getY());
		setTextColor(getColor());
		cout << (char)(getFigure() + '0') << endl;
	}
}