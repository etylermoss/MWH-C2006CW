#include "MapCountry.hpp"

#include <utility>
#include <sstream>

std::string getCountryPath(const std::string& name, enum CountryState state, int worldX, int worldY)
{
	std::stringstream countrySpritePath;
	countrySpritePath << "countries/country_";

	switch (state) {
	case CountryState::NORMAL:   countrySpritePath << "normal"; break;
	case CountryState::SELECTED: countrySpritePath << "selected"; break;
	case CountryState::COMPLETE: countrySpritePath << "complete"; break;
	}

	countrySpritePath << "_" << name << "_" << worldX << "x" << worldY << ".png";

	return countrySpritePath.str();
}

MapCountry::MapCountry(BaseEngine* engine, int level, std::string name, const std::string& nameInPath, int worldX, int worldY, int capitalX, int capitalY) :
	name(std::move(name)),
	level(level),
	spriteNormal(engine, getCountryPath(nameInPath, CountryState::NORMAL, worldX, worldY)),
	spriteSelected(engine, getCountryPath(nameInPath, CountryState::SELECTED, worldX, worldY)),
	spriteComplete(engine, getCountryPath(nameInPath, CountryState::COMPLETE, worldX, worldY)),
	currentState(CountryState::NORMAL),
	complete(false),
	worldX(worldX),
	worldY(worldY),
	capitalX(capitalX),
	capitalY(capitalY),
	width(spriteNormal.getWidth()),
	height(spriteNormal.getHeight())
{
}

MapCountry::~MapCountry() = default;

bool MapCountry::collides(int x, int y)
{
	return spriteNormal.getPixel(x - worldX, y - worldY).isAlpha();
}

std::string MapCountry::getName()
{
	return name;
}


