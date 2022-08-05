#include <fstream>
#include <cstring>
#include <iostream>
#include <limits>

#include "SaveGame.hpp"

void saveGameToFile(const SaveGame& saveGame)
{
	std::ofstream saveFile("moldovan_wine_hopper.savegame", std::ios::out | std::ios::trunc);

	if (!saveFile)
		return;

	saveFile << "[Moldovan Wine Hopper, Save-game]\n";
	saveFile << "player=" << saveGame.playerName << "\n";
	saveFile << "level=" << saveGame.level << "\n";
	saveFile << "score=" << saveGame.score << "\n";
}

std::optional<SaveGame> loadGameFromFile()
{
	std::ifstream saveFile("moldovan_wine_hopper.savegame", std::ios::in);
	char buf[256] = { 0 };

	if (!saveFile)
		return std::nullopt;

	SaveGame saveGame;

	// header
	saveFile.getline(buf, 255, '\n');
	if (strcmp(buf, "[Moldovan Wine Hopper, Save-game]") != 0)
		return std::nullopt;

	// player tag
	saveFile.getline(buf, 255, '=');
	if (strcmp(buf, "player") != 0)
		return std::nullopt;

	// player name
	saveFile.getline(buf, 255, '\n');
	if (strlen(buf) < 4 || strlen(buf) > 16)
		return std::nullopt;
	saveGame.playerName = buf;

	// level tag
	saveFile.getline(buf, 255, '=');
	if (strcmp(buf, "level") != 0)
		return std::nullopt;

	// level value
	saveFile.getline(buf, 255, '\n');
	if (strlen(buf) < 1)
		return std::nullopt;
	char* endPtrBuf;
	long int levelVal = strtol(buf, &endPtrBuf, 10);
	if (endPtrBuf == buf || levelVal < 1 || levelVal > 256)
		return std::nullopt;
	saveGame.level = (int) levelVal;

	// score tag
	saveFile.getline(buf, 255, '=');
	if (strcmp(buf, "score") != 0)
		return std::nullopt;

	// score value
	saveFile.getline(buf, 255, '\n');
	if (strlen(buf) < 1)
		return std::nullopt;
	long int scoreVal = strtol(buf, &endPtrBuf, 10);
	if (endPtrBuf == buf || scoreVal < 0 || scoreVal > std::numeric_limits<int>::max())
		return std::nullopt;
	saveGame.score = (int) scoreVal;

	return saveGame;
}