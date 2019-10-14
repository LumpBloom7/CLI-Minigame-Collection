#pragma once
#include <fstream>
#include <filesystem>
#include "../../_include/getExePath.h"

const std::filesystem::path configPath = std::filesystem::path(getExePath()) / "Configs" / "Snek.cfg";

struct SnekConfigStore {
	static void deserialize();
	static void serialize();

	static int playfieldWidth, playfieldHeight;
	static int simultaneousPellets;
	static int pelletValue;
	static int bonusValue;
	static double bonusChance;
	static int bonusDuration;
	static int snakeUpdateDelayMs;
	static int snekStartLength;
};