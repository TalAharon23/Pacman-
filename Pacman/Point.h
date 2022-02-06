#pragma once

class Point {

	int x;
	int y;

public:
	Point(int _x = 0, int _y = 0) : x(_x), y(_y) {};

	int getX() const { return x; }
	int getY() const { return y; }
	void setX(int _x) { x = _x; }
	void setY(int _y) { y = _y; }

	void setPoint(Point p) { x = p.x, y = p.y; }

	bool operator==(const Point& p) const {
		return((x == p.getX()) && (y == p.getY()));
	}
};
