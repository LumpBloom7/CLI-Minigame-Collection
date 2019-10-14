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
		<< snekStartLength << std::endl;
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
	}
}
int SnekConfigStore::playfieldWidth = 32;
int SnekConfigStore::playfieldHeight = 32;
int SnekConfigStore::simultaneousPellets = 1;
int SnekConfigStore::pelletValue = 5;
int SnekConfigStore::bonusValue = 25;
double SnekConfigStore::bonusChance = 0.2;
int SnekConfigStore::bonusDuration = 50;
int SnekConfigStore::snakeUpdateDelayMs = 16;
int SnekConfigStore::snekStartLength = 3;