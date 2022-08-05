#ifndef __H_MAP_COUNTRY__
#define __H_MAP_COUNTRY__

#include <string>

#include "lib/header.h"
#include "lib/BaseEngine.h"
#include "lib/SimpleImage.h"

#include "Surface.hpp"
#include "Render.hpp"
#include "SpriteManager.hpp"

enum class CountryState {
	NORMAL,
	SELECTED,
	COMPLETE,
};

class MapCountry
{
public:
	MapCountry(BaseEngine* engine, int level, std::string name, const std::string& nameInPath, int worldX, int worldY, int capitalX = 0, int capitalY = 0);
	~MapCountry();

	Surface& getSurface() {
		if (complete)
			return spriteComplete;
		switch (currentState) {
		case (CountryState::COMPLETE): return spriteComplete;
		case (CountryState::SELECTED): return spriteSelected;
		case (CountryState::NORMAL):
		default:                       return spriteNormal;
		}
	}

	bool updateSelected(bool isSelected) {
		auto oldState = currentState;

		if (isSelected) {
			currentState = CountryState::SELECTED;
		} else {
			currentState = CountryState::NORMAL;
		}

		return oldState != currentState;
	}

	bool getComplete() const {
		return complete;
	}
	void setComplete(bool newCOmplete) {
		complete = newCOmplete;
	}

	[[nodiscard]] bool collides(int worldX, int worldY);

	[[nodiscard]] std::string getName();
	[[nodiscard]] CountryState getState() { return currentState; };
	[[nodiscard]] int getWorldX() const { return worldX; };
	[[nodiscard]] int getWorldY() const { return worldY; };
	[[nodiscard]] int getWidth() const { return width; };
	[[nodiscard]] int getHeight() const { return height; };
	[[nodiscard]] int getCapitalX() const { return capitalX; };
	[[nodiscard]] int getCapitalY() const { return capitalY; };
	[[nodiscard]] int getLevel() const { return level; };

private:
	std::string name;
	Surface spriteNormal, spriteSelected, spriteComplete;
	CountryState currentState;
	int level;
	bool complete;
	int worldX, worldY;
	int width, height;
	int capitalX, capitalY;
};


#endif
