#include "Snek.h"

void SnekGame::play() {
	cligCore::console::clear();
	std::thread inputThread(
		[&] {
			while (!m_dead && !m_paused) {
				handleInput();
			}
		}
	);
	while (!m_dead) {
		if (!m_paused) {
			update();
			print();
		}
	}
	auto handle = inputThread.native_handle();
	inputThread.detach();
	TerminateThread(handle, 0);
}

void SnekGame::handleInput() {
	using namespace cligCore::input;
	switch (cligCore::input::getKeyInput()) {
	case Keys::Left:
		m_currDirection = (m_currDirection == Direction::right ? m_currDirection.load() : Direction::left);
		break;
	case Keys::Right:
		m_currDirection = (m_currDirection == Direction::left ? m_currDirection.load() : Direction::right);
		break;
	case Keys::Up:
		m_currDirection = (m_currDirection == Direction::down ? m_currDirection.load() : Direction::up);
		break;
	case Keys::Down:
		m_currDirection = (m_currDirection == Direction::up ? m_currDirection.load() : Direction::down);
		break;
	case Keys::Escape:
		m_paused = true;
		break;
	}
}

void SnekGame::update() {
	moveSnek();
	checkCollision();
	if (m_pellets < 5)
		spawnPellet();
	if (!m_bonusSpawned)
		spawnBonus();

	if (m_snake.back().currentPos != m_snake.back().previousPos)
		m_playfield[m_snake.back().previousPos.x][m_snake.back().previousPos.y] = CellContent::empty;
	m_playfield[m_snake.front().currentPos.x][m_snake.front().currentPos.y] = CellContent::snake;
}

void SnekGame::print() {
	cligCore::console::resetCursor();
	auto currentContent = CellContent::empty;
	std::cout << "Score: " << m_currScore << "\n";
	for (int j = 0; j < m_playfield[0].size(); ++j) {
		for (int i = 0; i < m_playfield.size(); ++i) {
			if (currentContent != m_playfield[i][j]) {
				currentContent = m_playfield[i][j];
				std::cout << contentColors[currentContent];
			}
			std::cout << "  ";
		}
		std::cout << "\n";
	}
	std::cout << rang::bg::reset << std::endl;
}

void SnekGame::initializePlayfield() {
	for (int i = 0; i < m_playfield[0].size(); ++i)
		m_playfield[i].front() = m_playfield[i].back() = CellContent::wall;
	for (int i = 0; i < m_playfield[0].size(); ++i)
		m_playfield.front()[i] = m_playfield.back()[i] = CellContent::wall;

	for (auto& piece : m_snake)
		m_playfield[piece.currentPos.x][piece.currentPos.y] = CellContent::snake;
}

void SnekGame::moveSnek() {
	m_snake.front().previousPos = m_snake.front().currentPos;
	switch (m_currDirection) {
	case Direction::left:
		--m_snake.front().currentPos.x;
		break;
	case Direction::right:
		++m_snake.front().currentPos.x;
		break;
	case Direction::up:
		--m_snake.front().currentPos.y;
		break;
	case Direction::down:
		++m_snake.front().currentPos.y;
		break;
	}

	for (auto piece = m_snake.begin() + 1; piece < m_snake.end(); ++piece)
		piece->previousPos = piece->currentPos, piece->currentPos = (piece - 1)->previousPos;
}

void SnekGame::checkCollision() {
	switch (m_playfield[m_snake.front().currentPos.x][m_snake.front().currentPos.y]) {
	case CellContent::pellet:
		m_snake.emplace_back(SnakePiece{ m_snake.back().previousPos,m_snake.back().previousPos });
		m_currScore += 5;
		--m_pellets;
		break;
	case CellContent::bonus:
		m_snake.emplace_back(SnakePiece{ m_snake.back().previousPos,m_snake.back().previousPos });
		m_currScore += 25;
		m_bonusSpawned = false;
		break;
	case CellContent::wall:
	case CellContent::snake:
		m_dead = true;
		break;
	}
}

void SnekGame::spawnPellet() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> gen(0, m_playfield.size() - 1);
	int x, y;
	do {
		x = gen(mt), y = gen(mt);
	} while (m_playfield[x][y] != CellContent::empty);
	m_playfield[x][y] = CellContent::pellet;
	++m_pellets;
}

void SnekGame::spawnBonus(double chance) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> gen(0, m_playfield.size() - 1);
	std::uniform_real_distribution<double> gen2(0.0, 99.00);
	if (gen2(mt) < chance) {
		int x, y;
		do {
			x = gen(mt), y = gen(mt);
		} while (m_playfield[x][y] != CellContent::empty);
		m_playfield[x][y] = CellContent::bonus;
		m_bonusSpawned = true;
	}
}
