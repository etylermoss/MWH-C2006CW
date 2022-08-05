#ifndef __H_SPRITE_MANAGER__
#define __H_SPRITE_MANAGER__

#include <string>

#include "lib/header.h"
#include "lib/BaseEngine.h"
#include "lib/SimpleImage.h"

enum class Sprites {
	FLAG,
	GROUND,
	PLATFORM,
	PLATFORM_RED,
};

class SpriteManager
{
public:
	explicit SpriteManager(BaseEngine& engine);
	~SpriteManager();

	[[nodiscard]] SimpleImage getSprite(enum Sprites sprite) const;

private:
	BaseEngine& engine;

	static std::string spriteToSpritePath(enum Sprites sprite);
};

#endif
