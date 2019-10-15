#pragma once
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include "../../_include/getExePath.h"
#include "Types.h"
#include "cligCore.h"

const std::filesystem::path configPath = std::filesystem::path(getExePath()) / "Configs" / "Snek.cfg";

static struct SnekConfigStore {
	SnekConfigStore() {
		contentColors[CellContent::bonus] = rang::bg::magenta;
		contentColors[CellContent::snake] = rang::bg::green;
		contentColors[CellContent::wall] = rang::bg::gray;
		contentColors[CellContent::pellet] = rang::bg::red;
	}
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

	static std::unordered_map<CellContent, rang::bg> contentColors;
};