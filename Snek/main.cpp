#include "cligCore.h"
#include "Snek.h"
void snekOptions();
void colorPicker(CellContent cellContent) {
	std::string defaultCol;
	switch (cellContent) {
	case CellContent::wall:
		defaultCol = "the walls. (Default is grey)";
		break;
	case CellContent::bonus:
		defaultCol = "the bonus. (Default is magenta)";
		break;
	case CellContent::pellet:
		defaultCol = "the pellet. (Default is red)";
		break;
	case CellContent::snake:
		defaultCol = "the snake. (Default is green)";
		break;
	}
	cligCore::types::Menu colorMenu({ "Black", "Blue","Cyan","Gray","Green","Magenta","Red","Yellow", "Back" }, "Pick a color for " + defaultCol);
	int result = 0;
	while (result != -1) {
		result = colorMenu.show();

		switch (result) {
		case 0:
			SnekConfigStore::contentColors[cellContent] = rang::bg::black; result = -1; break;
		case 1:
			SnekConfigStore::contentColors[cellContent] = rang::bg::blue; result = -1; break;
		case 2:
			SnekConfigStore::contentColors[cellContent] = rang::bg::cyan; result = -1; break;
		case 3:
			SnekConfigStore::contentColors[cellContent] = rang::bg::gray; result = -1; break;
		case 4:
			SnekConfigStore::contentColors[cellContent] = rang::bg::green; result = -1; break;
		case 5:
			SnekConfigStore::contentColors[cellContent] = rang::bg::magenta; result = -1; break;
		case 6:
			SnekConfigStore::contentColors[cellContent] = rang::bg::red; result = -1; break;
		case 7:
			SnekConfigStore::contentColors[cellContent] = rang::bg::yellow; result = -1; break;
		case-1:case 8:
			result = -1;
			break;
		}
	}

}
void snekMain() {
	SnekConfigStore::deserialize();
	cligCore::types::Menu snekMenu({ "Start", "Options", "Return to main menu"}, "Snek menu");

	int result = 0;
	while (result != -1) {
		result = snekMenu.show();

		switch (result) {
		case 0: {
			SnekGame game;
			game.play();
			break;
		}
		case 1:
			snekOptions();
			break;
		case 2:
		case-1:
			result = -1;
			break;
		}
	}
	SnekConfigStore::serialize();
}
void snekOptions() {
	cligCore::types::Menu snekOptions(
		{
			"Playfield width",
			"Playfield height",
			"Starting length",
			"Number of simultaneous pellets",
			"Pellet score value",
			"Bonus score value",
			"Bonus chance",
			"Bonus duration",
			"Game speed",
			"Wall color",
			"Bonus color",
			"Pellet color",
			"Snake color",
			"Back"
		}, "Snek configuration");

	int result = 0;
	while (result != -1) {
		result = snekOptions.show();
		switch (result) {
		case 0: {
			cligCore::types::Range chooser(10, cligCore::console::getConsoleWidth() / 2 - 2, SnekConfigStore::playfieldWidth);
			chooser.showChooser("Set your desired playfield width (Default is 32)");
			SnekConfigStore::playfieldWidth = chooser.getCurrent();
			break;
		}
		case 1: {
			cligCore::types::Range chooser(10, cligCore::console::getConsoleHeight() - 3, SnekConfigStore::playfieldHeight);
			chooser.showChooser("Set your desired playfield height (Default is 32)");
			SnekConfigStore::playfieldHeight = chooser.getCurrent();
			break;
		}
		case 2: {
			cligCore::types::Range chooser(1, SnekConfigStore::playfieldHeight * SnekConfigStore::playfieldWidth / 2, SnekConfigStore::snekStartLength);
			chooser.showChooser("Set your desired starting length (Default is 3)");
			SnekConfigStore::snekStartLength = chooser.getCurrent();
			break;
		}
		case 3: {
			cligCore::types::Range chooser(1, 10, SnekConfigStore::simultaneousPellets);
			chooser.showChooser("Set the amount of simultaneous pellets that can be on screen (Default is 1)");
			SnekConfigStore::simultaneousPellets = chooser.getCurrent();
			break;
		}
		case 4: {
			cligCore::types::Range chooser(1, 100, SnekConfigStore::pelletValue);
			chooser.showChooser("Set the value of the pellet (Default is 5)");
			SnekConfigStore::pelletValue = chooser.getCurrent();
			break;
		}
		case 5: {
			cligCore::types::Range chooser(1, 100, SnekConfigStore::bonusValue);
			chooser.showChooser("Set the value of the bonus (Default is 25)");
			SnekConfigStore::bonusValue = chooser.getCurrent();
			break;
		}
		case 6: {
			double tmp = -1;
			cligCore::console::clear();
			std::cout << "Please enter the chances (in \%) of a bonus spawning every frame (Default is 0.02\%)" << std::endl;
			std::cin >> tmp;
			while (!std::cin) {
				std::cout << rang::fg::red << "ERROR: Invalid input" << std::endl << std::endl << rang::fg::reset;
#undef max
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Please enter the chances (in \%) of a bonus spawning every frame" << std::endl;
				std::cin >> tmp;
			}
			SnekConfigStore::bonusChance = tmp;
			break;
		}
		case 7: {
			cligCore::types::Range chooser(120, 3000, SnekConfigStore::bonusDuration);
			chooser.showChooser("Set the duration the bonus stays on screen in frames(Default is 300)");
			SnekConfigStore::bonusDuration = chooser.getCurrent();
			break;
		}
		case 8: {
			cligCore::types::Menu menu({ "1fps", "3fps", "10fps", "30fps", "60fps" , "Uncapped (Good luck champ)", "Cancel" }, "Select the framerate(Affects game speed, Default is 10fps)");
			int result = 0;
			while (result != -1) {
				result = menu.show();
				switch (result) {
				case 0: SnekConfigStore::snakeUpdateDelayMs = 1000; result = -1; break;
				case 1: SnekConfigStore::snakeUpdateDelayMs = 333; result = -1; break;
				case 2: SnekConfigStore::snakeUpdateDelayMs = 100; result = -1; break;
				case 3: SnekConfigStore::snakeUpdateDelayMs = 33; result = -1; break;
				case 4: SnekConfigStore::snakeUpdateDelayMs = 16; result = -1; break;
				case 5: SnekConfigStore::snakeUpdateDelayMs = 0; result = -1; break;
				case 6:
				case -1: result = -1;
				}
			}
			break;
		}
		case 9: colorPicker(CellContent::wall); break;
		case 10: colorPicker(CellContent::bonus); break;
		case 11: colorPicker(CellContent::pellet); break;
		case 12: colorPicker(CellContent::snake); break;
		case -1:
		case 13:
			result = -1;
		}

	}
}