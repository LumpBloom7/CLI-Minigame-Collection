#include "SnekConfig.h"

void SnekConfigStore::serialize() {
	if (!std::filesystem::exists(configPath.parent_path()))
		std::filesystem::create_directories(configPath.parent_path());
	std::ofstream file(configPath);
	file << std::hex
		<< playfieldWidth << std::endl
		<< playfieldHeight << std::endl
		<< simultaneousPellets << std::endl
		<< pelletValue << std::endl
		<< bonusChance << std::endl
		<< bonusDuration << std::endl
		<< snakeUpdateDelayMs << std::endl
		<< snekStartLength << std::endl
		<< contentColors[CellContent::bonus] << std::endl
		<< contentColors[CellContent::snake] << std::endl
		<< contentColors[CellContent::wall] << std::endl
		<< contentColors[CellContent::pellet] << std::endl;

}
void SnekConfigStore::deserialize() {
	if (std::filesystem::exists(configPath)) {
		std::ifstream file(configPath);
		file >> std::hex
			>> playfieldWidth
			>> playfieldHeight
			>> simultaneousPellets
			>> pelletValue
			>> bonusChance
			>> bonusDuration
			>> snakeUpdateDelayMs
			>> snekStartLength;
		int colorCode;
		file >> colorCode;
		contentColors[CellContent::bonus] = static_cast<rang::bg>(colorCode);
		file >> colorCode;
		contentColors[CellContent::snake] = static_cast<rang::bg>(colorCode);
		file >> colorCode;
		contentColors[CellContent::wall] = static_cast<rang::bg>(colorCode);
		file >> colorCode;
		contentColors[CellContent::pellet] = static_cast<rang::bg>(colorCode);
	}
}

int SnekConfigStore::playfieldWidth = 32;
int SnekConfigStore::playfieldHeight = 32;
int SnekConfigStore::simultaneousPellets = 1;
int SnekConfigStore::pelletValue = 5;
int SnekConfigStore::bonusValue = 25;
double SnekConfigStore::bonusChance = 0.2;
int SnekConfigStore::bonusDuration = 300;
int SnekConfigStore::snakeUpdateDelayMs = 16;
int SnekConfigStore::snekStartLength = 3;
std::unordered_map<CellContent, rang::bg> SnekConfigStore::contentColors{
	{CellContent::bonus, rang::bg::magenta},
	{CellContent::snake,  rang::bg::green },
	{CellContent::wall, rang::bg::gray},
	{CellContent::pellet, rang::bg::red}
};
