#ifndef __H_LOAD_LEVEL__
#define __H_LOAD_LEVEL__

#include <string>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>

enum class Block {
	NONE,
	GROUND,
	PLATFORM,
	PLATFORM_RED,
};

typedef std::tuple<int, int> XY_t;

struct tuple_2_hash
{
	template <class T1, class T2>
	size_t operator() (const std::tuple<T1, T2>& x) const
	{
		return get<0>(x) ^ get<1>(x);
	}
};

class Level
{
public:
	Level();
	Level(int levelID, std::string country);

	[[nodiscard]] std::string getTitle() const;
	[[nodiscard]] std::string getCountry() const;
	[[nodiscard]] int getID() const;
	[[nodiscard]] XY_t getSize() const;
	[[nodiscard]] XY_t getSpawn() const;
	[[nodiscard]] XY_t getEnd() const;
	[[nodiscard]] XY_t getEndSize() const;

	[[nodiscard]] enum Block getBlockAt(int x, int y) const;
	bool setBlockAt(int x, int y, enum Block block);

private:
	std::string title, country;
	int id;
	int width, height;
	int spawnX, spawnY;
	int endX, endY, endW, endH;
	std::unordered_map<XY_t, enum Block, tuple_2_hash> blocks;
};


#endif
