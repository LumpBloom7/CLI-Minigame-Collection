#pragma once
#include <vector>
#include <atomic>
#include <random>
#include <unordered_map>
#include "cligCore.h"
#include <thread>

void snekMain();

struct Coord {
	bool operator==(const Coord& rhs) {
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(const Coord& rhs) {
		return !operator==(rhs);
	}

	int x = 0, y = 0;
};
struct SnakePiece {
	Coord currentPos, previousPos;
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

class SnekGame {
	using Snake = std::vector<SnakePiece>;
	using Playfield = std::vector<std::vector<CellContent>>;

public:
	SnekGame() {
		initializePlayfield();
		contentColors[CellContent::bonus] = rang::bg::magenta;
		contentColors[CellContent::empty] = rang::bg::reset;
		contentColors[CellContent::snake] = rang::bg::green;
		contentColors[CellContent::wall] = rang::bg::gray;
		contentColors[CellContent::pellet] = rang::bg::red;
	}
	void play();
	void handleInput();
	void update();
	void print();

private:
	void initializePlayfield();
	void moveSnek();
	void checkCollision();
	void spawnPellet();
	void spawnBonus(double chance = 10.0);

	Playfield m_playfield = std::vector<std::vector<CellContent>>(32, std::vector<CellContent>(32, CellContent::empty));
	Snake m_snake{ {{2,1}},{{1,1}} };
	std::atomic<Direction> m_currDirection = Direction::right;
	bool m_paused = false;
	int m_currScore = 0;
	int m_pellets = 0;
	bool m_bonusSpawned = false;
	bool m_dead = false;

	std::unordered_map<CellContent, rang::bg> contentColors;

};