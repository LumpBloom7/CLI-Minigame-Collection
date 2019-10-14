#pragma once
#include <vector>
#include <deque>
#include <atomic>
#include <random>
#include <unordered_map>
#include "cligCore.h"
#include <thread>
#include "SnekConfig.h"

void snekMain();

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

class SnekGame {
	using Pellet = Coord;
	using PelletList = std::deque<Pellet>;
	using Bonus = Coord;
	using SnakePiece = Coord;
	using Snake = std::deque<SnakePiece>;
	using Playfield = std::unordered_map<int, std::unordered_map<int, std::unordered_map<CellContent, bool>>>;

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
	Direction handleInput();
	void update();
	void print(bool reprint = false);

private:
	void printStats();
	void printSnake();
	void printBorder();
	void printPellets();
	void initializePlayfield();
	void moveSnek();
	void checkCollision();
	void spawnPellet();
	void spawnBonus();

	Snake m_snake{};
	std::atomic<Direction> m_currDirection = Direction::right;
	bool m_paused = false;
	int m_currScore = 0;
	bool m_dead = false;
	PelletList m_pellets;
	Bonus m_bonus{ -1,-1 };
	int m_bonusTimer = 50;
	Playfield m_playfield;

	std::unordered_map<CellContent, rang::bg> contentColors;

};