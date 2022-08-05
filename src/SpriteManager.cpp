#include <string>

#include "SpriteManager.hpp"

SpriteManager::SpriteManager(BaseEngine& engine) :
	engine(engine)
{
}

SpriteManager::~SpriteManager()
{
}

SimpleImage SpriteManager::getSprite(enum Sprites sprite) const
{
	return engine.loadImage(spriteToSpritePath(sprite), true);
}

std::string SpriteManager::spriteToSpritePath(enum Sprites sprite)
{
	switch (sprite) {
	case Sprites::FLAG:         return "flag_160x120.png";
	case Sprites::GROUND:       return "ground_16x16.png";
	case Sprites::PLATFORM:     return "platform_16x16.png";
	case Sprites::PLATFORM_RED: return "platform_red_16x16.png";
	}

	return "";
}
