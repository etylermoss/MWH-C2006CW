#include <optional>

#include "Level.hpp"

std::optional<enum Block> strToBlock(const std::string& str)
{
	if (str == "GROUND")
		return Block::GROUND;
	else if (str == "PLATFORM")
		return Block::PLATFORM;
	else if (str == "PLATFORM_RED")
		return Block::PLATFORM_RED;

	return std::nullopt;
}

Level::Level() :
	id(0),
	width(0),
	height(0),
	spawnX(0),
	spawnY(0),
	endX(0),
	endY(0),
	endW(0),
	endH(0)
{
}

Level::Level(int levelID, std::string _country) :
	country(std::move(_country)),
	endW(16),
	endH(32)
{
	std::stringstream filePath; filePath << "moldovan_wine_hopper." << levelID << ".level";
	std::ifstream loadFile(filePath.str(), std::ios::in);

	if (!loadFile)
		throw std::runtime_error("Level::Level(), could not load level file");

	char buf[256] = { 0 };
	char* bufEndPtr;

	// header
	loadFile.getline(buf, 255, '\n');
	if (strcmp(buf, "[Moldovan Wine Hopper, Level-data]") != 0)
		throw std::runtime_error("Level::Level(), missing level header");

	// title tag
	loadFile.getline(buf, 255, '=');
	if (strcmp(buf, "title") != 0)
		throw std::runtime_error("Level::Level(), missing title tag");

	// title value
	loadFile.getline(buf, 255, '\n');
	if (strlen(buf) < 1 || strlen(buf) > 64)
		throw std::runtime_error("Level::Level(), missing title value");
	title = buf;

	// level tag
	loadFile.getline(buf, 255, '=');
	if (strcmp(buf, "level") != 0)
		throw std::runtime_error("Level::Level(), missing level tag");

	// level value
	loadFile.getline(buf, 255, '\n');
	if (strlen(buf) < 1)
		throw std::runtime_error("Level::Level(), missing level value");
	int levelVal = (int) strtol(buf, &bufEndPtr, 10);
	if (bufEndPtr == buf || levelVal < 1 || levelVal > 256 || levelVal != levelID)
		throw std::runtime_error("Level::Level(), missing (valid) level value");
	id = levelVal;

	// size tag
	loadFile.getline(buf, 255, '=');
	if (strcmp(buf, "size") != 0)
		throw std::runtime_error("Level::Level(), missing size tag");

	// size value
	loadFile.getline(buf, 255, ',');
	if (strlen(buf) < 1)
		throw std::runtime_error("Level::Level(), missing size X value");
	int sizeX = (int) strtol(buf, &bufEndPtr, 10);
	if (bufEndPtr == buf || sizeX < 1 || sizeX > 1024)
		throw std::runtime_error("Level::Level(), missing (valid) size X");
	width = sizeX;
	loadFile.getline(buf, 255, '\n');
	if (strlen(buf) < 1)
		throw std::runtime_error("Level::Level(), missing size Y value");
	int sizeY = (int) strtol(buf, &bufEndPtr, 10);
	if (bufEndPtr == buf || sizeY < 1 || sizeY > 1024)
		throw std::runtime_error("Level::Level(), missing (valid) size Y");
	height = sizeY;

	// spawn tag
	loadFile.getline(buf, 255, '=');
	if (strcmp(buf, "spawn") != 0)
		throw std::runtime_error("Level::Level(), missing spawn tag");

	// spawn value
	loadFile.getline(buf, 255, ',');
	if (strlen(buf) < 1)
		throw std::runtime_error("Level::Level(), missing spawn X value");
	int _spawnX = (int) strtol(buf, &bufEndPtr, 10);
	if (bufEndPtr == buf || _spawnX < 0 || _spawnX > 1024)
		throw std::runtime_error("Level::Level(), missing (valid) spawn X");
	spawnX = _spawnX;
	loadFile.getline(buf, 255, '\n');
	if (strlen(buf) < 1)
		throw std::runtime_error("Level::Level(), missing spawn Y value");
	int _spawnY = (int) strtol(buf, &bufEndPtr, 10);
	if (bufEndPtr == buf || _spawnY < 0 || _spawnY > 1024)
		throw std::runtime_error("Level::Level(), missing (valid) spawn Y");
	spawnY = _spawnY;

	// end tag
	loadFile.getline(buf, 255, '=');
	if (strcmp(buf, "end") != 0)
		throw std::runtime_error("Level::Level(), missing end tag");

	// spawn value
	loadFile.getline(buf, 255, ',');
	if (strlen(buf) < 1)
		throw std::runtime_error("Level::Level(), missing end X value");
	int _endX = (int) strtol(buf, &bufEndPtr, 10);
	if (bufEndPtr == buf || _endX < 0 || _endX > 1024)
		throw std::runtime_error("Level::Level(), missing (valid) end X");
	endX = _endX;
	loadFile.getline(buf, 255, '\n');
	if (strlen(buf) < 1)
		throw std::runtime_error("Level::Level(), missing end Y value");
	int _endY = (int) strtol(buf, &bufEndPtr, 10);
	if (bufEndPtr == buf || _endY < 0 || _endY > 1024)
		throw std::runtime_error("Level::Level(), missing (valid) end Y");
	endY = _endY;

	// blocks tag
	loadFile.getline(buf, 255, '=');
	if (strcmp(buf, "blocks") != 0)
		throw std::runtime_error("Level::Level(), missing level tag");

	// skip to blocks data
	loadFile.getline(buf, 0, '\n');

	// blocks
	// loop until end of file
	while (true) {
		// get first value
		loadFile.getline(buf, 255, ',');
		if (strlen(buf) < 1)
			break;
		if (buf[0] == 'h') {
			int x = (int) strtol(buf + 1, &bufEndPtr, 10);
			if (bufEndPtr == buf)
				throw std::runtime_error("Level::Level(), missing x in horizontal block range");
			loadFile.getline(buf, 255, ',');
			if (strlen(buf) < 1)
				throw std::runtime_error("Level::Level(), missing y in horizontal block range");
			int y = (int) strtol(buf, &bufEndPtr, 10);
			if (bufEndPtr == buf)
				throw std::runtime_error("Level::Level(), missing y in horizontal block range");
			loadFile.getline(buf, 255, ',');
			if (strlen(buf) < 1)
				throw std::runtime_error("Level::Level(), missing l in horizontal block range");
			int l = (int) strtol(buf, &bufEndPtr, 10);
			if (bufEndPtr == buf || l == 0)
				throw std::runtime_error("Level::Level(), missing l in horizontal block range");
			loadFile.getline(buf, 255, '\n');
			if (strlen(buf) < 1 || strlen(buf) > 64)
				throw std::runtime_error("Level::Level(), missing block in horizontal block range");
			auto block = strToBlock(buf);
			if (!block)
				throw std::runtime_error("Level::Level(), missing (valid) block in horizontal block range");
			for (int i = abs(l) - 1; i >= 0; --i)
				setBlockAt(x + (l < 0 ? -i : i), y, *block);
		} else if (buf[0] == 'v') {
			int x = (int) strtol(buf + 1, &bufEndPtr, 10);
			if (bufEndPtr == buf)
				throw std::runtime_error("Level::Level(), missing x in vertical block range");
			loadFile.getline(buf, 255, ',');
			if (strlen(buf) < 1)
				throw std::runtime_error("Level::Level(), missing y in vertical block range");
			int y = (int) strtol(buf, &bufEndPtr, 10);
			if (bufEndPtr == buf)
				throw std::runtime_error("Level::Level(), missing y in vertical block range");
			loadFile.getline(buf, 255, ',');
			if (strlen(buf) < 1)
				throw std::runtime_error("Level::Level(), missing l in vertical block range");
			int l = (int) strtol(buf, &bufEndPtr, 10);
			if (bufEndPtr == buf || l == 0)
				throw std::runtime_error("Level::Level(), missing l in vertical block range");
			loadFile.getline(buf, 255, '\n');
			if (strlen(buf) < 1 || strlen(buf) > 64)
				throw std::runtime_error("Level::Level(), missing block in vertical block range");
			auto block = strToBlock(buf);
			if (!block)
				throw std::runtime_error("Level::Level(), missing (valid) block in vertical block range");
			for (int i = abs(l) - 1; i >= 0; --i)
				setBlockAt(x, y + (l < 0 ? -i : i), *block);
		} else {
			int x = (int) strtol(buf, &bufEndPtr, 10);
			if (bufEndPtr == buf)
				throw std::runtime_error("Level::Level(), missing x in block");
			loadFile.getline(buf, 255, ',');
			if (strlen(buf) < 1)
				throw std::runtime_error("Level::Level(), missing y in block");
			int y = (int) strtol(buf, &bufEndPtr, 10);
			if (bufEndPtr == buf)
				throw std::runtime_error("Level::Level(), missing y in block");;
			loadFile.getline(buf, 255, '\n');
			if (strlen(buf) < 1 || strlen(buf) > 64)
				throw std::runtime_error("Level::Level(), missing block in block");
			auto block = strToBlock(buf);
			if (!block)
				throw std::runtime_error("Level::Level(), missing (valid) block in block");
			setBlockAt(x, y, *block);
		}
	}
}

std::string Level::getTitle() const
{
	return title;
}

std::string Level::getCountry() const
{
	return country;
}

int Level::getID() const
{
	return id;
}

XY_t Level::getSize() const
{
	return std::make_tuple(width, height);
}

XY_t Level::getSpawn() const
{
	return std::make_tuple(spawnX, spawnY);
}

XY_t Level::getEnd() const
{
	return std::make_tuple(endX, endY);
}

XY_t Level::getEndSize() const
{
	return std::make_tuple(endW, endH);
}

enum Block Level::getBlockAt(int x, int y) const
{
	auto block = blocks.find(std::make_tuple(x, y));
	if (block == blocks.end())
		return Block::NONE;
	return block->second;
}

bool Level::setBlockAt(int x, int y, enum Block block)
{
	if (block == Block::NONE)
		return false;
	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;
	blocks[std::make_tuple(x, y)] = block;
	return true;
}

