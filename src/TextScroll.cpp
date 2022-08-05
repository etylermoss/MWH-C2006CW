#include "Tick.hpp"
#include "Render.hpp"
#include "Surface.hpp"

#include "TextScroll.hpp"

TextScroll::TextScroll(TimeManager& timeManager) :
	Tick(),
	Render(),
	timeManager(timeManager),
	texts(0),
	timeStep(0, 1),
	posX(0),
	posY(0),
	colour(0xFFFFFF),
	font(nullptr),
	timePrev(),
	textsProgress(1)
{
}

TextScroll::TextScroll(TimeManager& timeManager, const Time& timeStep, int posX, int posY, const Colour& colour, Font* font) :
	Tick(),
	Render(),
	timeManager(timeManager),
	texts(0),
	timeStep(timeStep),
	posX(posX),
	posY(posY),
	colour(colour),
	font(font),
	timePrev(),
	textsProgress(1)
{
}

TextScroll::~TextScroll() = default;

void TextScroll::tick()
{
	Time timeCurr = timeManager.getTime();

	if (timePrev.differenceTo(timeCurr) > timeStep && textsProgress <= texts.size()) {
		timePrev = timeCurr;
		textsProgress++;
	}

	if (textsProgress > texts.size())
		this->setFinished(true);
}

void TextScroll::render(Surface& viewSurface)
{
	if (this->getFinished())
		return;

	viewSurface.getDrawingSurface().drawFastString(posX, posY, createCurrentText().c_str(), colour.getColourVal(), font); // TODO: replace
}

std::string TextScroll::createCurrentText() const
{
	std::string textJoined;

	for (int i = 0; i < texts.size() && i < textsProgress; i++)
		textJoined.append(texts[i]);

	return textJoined;
}

