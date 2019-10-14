#include "Snek.h"

void SnekGame::play() {
	cligCore::console::clear();
	print(true);
	std::thread inputThread(
		[&] {
			while (!m_dead && !m_paused) {
				handleInput();
			}
		}
	);
	while (!m_dead) {
		if (!m_paused) {
			std::this_thread::sleep_for(std::chrono::milliseconds(SnekConfigStore::snakeUpdateDelayMs));
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
	auto oldBack = m_snake.back();
	moveSnek();
	checkCollision();
	if (m_pellets.size() < SnekConfigStore::simultaneousPellets)
		spawnPellet();
	if (m_bonus.x != -1)
		spawnBonus();

	if (m_snake.back() != oldBack) {
		Coord topLeft(cligCore::console::getConsoleWidth() / 2 - SnekConfigStore::playfieldWidth, 2);
		cligCore::console::moveCursor(topLeft.x + oldBack.x * 2, topLeft.y + oldBack.y);
		std::cout << "  ";
		m_playfield[oldBack.x][oldBack.y][CellContent::snake] = false;
	}
}

void SnekGame::print(bool reprint) {
	printStats();
	if (reprint) {
		printBorder();
		printPellets();
	}
	printSnake();
	std::cout << rang::bg::reset;
}

void SnekGame::printStats() {
	cligCore::console::moveCursor(0, 0);
	std::string score = "Score: " + std::to_string(m_currScore);
	std::cout << score << std::endl;

	std::string bonus = "Bonus: " + std::to_string(m_bonusTimer);
	cligCore::console::moveCursor(cligCore::console::getConsoleWidth() / 2 - bonus.length() / 2, 0);
	std::cout << bonus << std::endl;

	std::string length = "Length: " + std::to_string(m_snake.size());
	cligCore::console::moveCursor(cligCore::console::getConsoleWidth() - length.length(), 0);
	std::cout << length << std::endl;

}

void SnekGame::printSnake() {
	std::cout << contentColors[CellContent::snake];
	Coord topLeft(cligCore::console::getConsoleWidth() / 2 - SnekConfigStore::playfieldWidth, 2);
	cligCore::console::moveCursor(topLeft.x + m_snake.front().x * 2, topLeft.y + m_snake.front().y);
	std::cout << "  ";
}

void SnekGame::printBorder() {
	std::cout << contentColors[CellContent::wall];
	Coord topLeft(cligCore::console::getConsoleWidth() / 2 - (SnekConfigStore::playfieldWidth + 2), 1);
	for (int i = 0; i < SnekConfigStore::playfieldWidth * 2 + 4; ++i) {
		cligCore::console::moveCursor(topLeft.x + i, topLeft.y);
		std::cout << " ";
		cligCore::console::moveCursor(topLeft.x + i, topLeft.y + SnekConfigStore::playfieldHeight + 1);
		std::cout << " ";
	}
	for (int i = 0; i < SnekConfigStore::playfieldHeight + 1; ++i) {
		cligCore::console::moveCursor(topLeft.x, topLeft.y + i);
		std::cout << "  ";
		cligCore::console::moveCursor(topLeft.x + SnekConfigStore::playfieldWidth * 2 + 2, topLeft.y + i);
		std::cout << "  ";
	}
}

void SnekGame::printPellets() {
	std::cout << contentColors[CellContent::pellet];
	Coord topLeft(cligCore::console::getConsoleWidth() / 2 - SnekConfigStore::playfieldWidth, 2);
	for (auto& pellet : m_pellets) {
		cligCore::console::moveCursor(topLeft.x + pellet.x * 2, topLeft.y + pellet.y);
		std::cout << "  ";
	}

	if (m_bonus.x != -1) {
		std::cout << contentColors[CellContent::bonus];
		cligCore::console::moveCursor(topLeft.x + m_bonus.x * 2, topLeft.y + m_bonus.y);
		std::cout << "  ";
	}
}

void SnekGame::initializePlayfield() {
}

void SnekGame::moveSnek() {
	auto newFront = m_snake.front();
	m_playfield[newFront.x][newFront.y][CellContent::snake] = true;
	switch (m_currDirection) {
	case Direction::left:
		--newFront.x;
		break;
	case Direction::right:
		++newFront.x;
		break;
	case Direction::up:
		--newFront.y;
		break;
	case Direction::down:
		++newFront.y;
		break;
	}
	m_snake.emplace_front(newFront);
	m_snake.pop_back();
}

void SnekGame::checkCollision() {
	for (auto pelletIt = m_pellets.begin(); pelletIt < m_pellets.end(); ++pelletIt)
		if (*pelletIt == m_snake.front()) {
			m_currScore += SnekConfigStore::pelletValue;
			m_snake.emplace_back(m_snake.back());
			m_pellets.erase(pelletIt);
			return;
		}

	if (m_bonus == m_snake.front()) {
		m_currScore += SnekConfigStore::bonusValue;
		m_bonus = Bonus(-1, -1);
		return;
	}

	if ((m_snake.front().x == -1 || m_snake.front().x == SnekConfigStore::playfieldWidth)
		|| (m_snake.front().y == -1 || m_snake.front().y == SnekConfigStore::playfieldHeight)
		|| m_playfield[m_snake.front().x][m_snake.front().y][CellContent::snake])
		m_dead = true;
}

void SnekGame::spawnPellet() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> genX(0, SnekConfigStore::playfieldWidth - 1);
	std::uniform_int_distribution<int> genY(0, SnekConfigStore::playfieldHeight - 1);
	Pellet newPellet;
	do {
		newPellet = Pellet(genX(mt), genY(mt));
	} while (
		m_bonus == newPellet ||
		m_playfield[newPellet.x][newPellet.y][CellContent::pellet] ||
		m_playfield[newPellet.x][newPellet.y][CellContent::snake]);
	m_pellets.emplace_back(newPellet);
	m_playfield[newPellet.x][newPellet.y][CellContent::pellet] = true;

	Coord topLeft(cligCore::console::getConsoleWidth() / 2 - SnekConfigStore::playfieldWidth, 2);
	cligCore::console::moveCursor(topLeft.x + newPellet.x * 2, topLeft.y + newPellet.y);
	std::cout << contentColors[CellContent::pellet] << "  ";
}

void SnekGame::spawnBonus() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> genX(0, SnekConfigStore::playfieldWidth - 1);
	std::uniform_int_distribution<int> genY(0, SnekConfigStore::playfieldHeight - 1);
	std::uniform_real_distribution<double> genChance(0.0, 100.0);
	Bonus newBonus;
	if (genChance(mt) < SnekConfigStore::bonusChance) {
		do {
			newBonus = Bonus(genX(mt), genY(mt));
		} while (
			m_playfield[newBonus.x][newBonus.y][CellContent::pellet] ||
			m_playfield[newBonus.x][newBonus.y][CellContent::snake]);
		m_bonus = newBonus;
		m_bonusTimer = SnekConfigStore::bonusDuration;

		Coord topLeft(cligCore::console::getConsoleWidth() / 2 - SnekConfigStore::playfieldWidth, 2);
		cligCore::console::moveCursor(topLeft.x + m_bonus.x * 2, topLeft.y + m_bonus.y);
		std::cout << contentColors[CellContent::bonus] << "  ";
	}
}
