#ifndef __H_MY_TILE_MANAGER__
#define __H_MY_TILE_MANAGER__

using namespace std;

#include "lib/header.h"
#include "lib/BaseEngine.h"
#include "lib/TileManager.h"

#include "Constants.hpp"
#include "Surface.hpp"
#include "Level.hpp"

class MyTileManager
{
public:
	explicit MyTileManager(BaseEngine* engine) :
		engine(engine),
		level(nullptr),
		tileGround(engine, "ground_16x16.png"),
		tilePlatform(engine, "platform_16x16.png"),
		tilePlatformRed(engine, "platform_red_16x16.png")
	{
	}

	void loadLevel(Level& newLevel)
	{
		level = &newLevel;
	}

	void render(Surface& viewSurface)
	{
		auto size = level->getSize();
		for (int x = 0; x < get<0>(size); ++x) {
			for (int y = 0; y < get<1>(size); ++y) {
				renderTile(viewSurface, x, y, level->getBlockAt(x, y));
			}
		}
	}

	void renderTile(Surface& viewSurface, int mapX, int mapY, enum Block block)
	{
		if (block == Block::NONE)
			return;

		int x = mapX * TILE_SIZE;
		int y = mapY * TILE_SIZE;

		viewSurface.drawRectangleFrom(blockToSurface(block), x, y, TILE_SIZE, TILE_SIZE);
	}

	Surface& blockToSurface(enum Block block) {
		switch (block) {
		case Block::NONE:
		case Block::GROUND:       return tileGround;
		case Block::PLATFORM:     return tilePlatform;
		case Block::PLATFORM_RED: return tilePlatformRed;
		}
	}


private:
	BaseEngine* engine;
	Level* level;
	Surface tileGround, tilePlatform, tilePlatformRed;
};

#endif
