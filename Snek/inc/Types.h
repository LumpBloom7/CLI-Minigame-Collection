#pragma once

struct Coord {
	Coord(int x = 0, int y = 0) : x(x), y(y) {}
	bool operator==(const Coord& rhs) {
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(const Coord& rhs) {
		return !operator==(rhs);
	}

	int x = 0, y = 0;
};

enum class CellContent {
	empty,
	wall,
	snake,
	pellet,
	bonus
};

enum class Direction {
	up,
	down,
	left,
	right
};