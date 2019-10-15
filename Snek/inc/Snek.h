#pragma once
#include <vector>
#include <deque>
#include <atomic>
#include <random>
#include <unordered_map>
#include "cligCore.h"
#include <thread>
#include "SnekConfig.h"
#include "Types.h"

void snekMain();

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


};