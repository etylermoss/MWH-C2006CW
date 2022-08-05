#ifndef __H_SAVE_GAME__
#define __H_SAVE_GAME__

#include <string>
#include <optional>

struct SaveGame
{
	std::string playerName;
	int level;
	int score;
};

void saveGameToFile(const SaveGame& saveGame);
std::optional<SaveGame> loadGameFromFile();

#endif
