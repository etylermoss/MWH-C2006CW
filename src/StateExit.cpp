#include "lib/header.h"

#include "InputManager.hpp"
#include "FontUtil.hpp"
#include "StateMenu.hpp"
#include "Time.hpp"

#include "StateExit.hpp"

StateExitRequirements::StateExitRequirements() :
	StateRequirements(StateKey::STATE_EXIT)
{
}

StateExitRequirements::~StateExitRequirements() = default;

StateExit::StateExit(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil) :
	State(StateKey::STATE_EXIT),
	inputManager(inputManager),
	spriteManager(spriteManager),
	timeManager(timeManager),
	fontUtil(fontUtil),
	textScroll(timeManager, Time(500), TILE_SIZE - 1, TILE_SIZE - 1, Colour::WHITE, fontUtil.getFont(FontSize::MEDIUM)),
	foreground(engine, VIEW_WIDTH, VIEW_HEIGHT),
	background(engine, VIEW_WIDTH, VIEW_HEIGHT)
{
	for (int i = 0; i < MAP_WIDTH; ++i) {
		for (int j = 0; j < MAP_HEIGHT; ++j) {
			Colour colour = (i + j) % 2 ? Colour::BLACK : Colour(0x111111);
			background.drawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, colour);
		}
	}
}

StateExit::~StateExit() = default;

void StateExit::initialize(std::unique_ptr<StateRequirements> baseRequirements)
{
	StateExitRequirements requirements = dynamic_cast<StateExitRequirements&>(*baseRequirements);

	textScroll.start('A', 'd', 'i', 'o', '!', ' ', ' ', ' ');
}

void StateExit::deinitialize()
{
}

void StateExit::tick()
{
	textScroll.tick();

	if (textScroll.getFinished())
		this->setShouldExit(true);
}

void StateExit::render(Surface& viewSurface)
{
	textScroll.render(background);

	viewSurface.drawFromSurface(background);
}

std::unique_ptr<StateRequirements> StateExit::getNextStateRequirements()
{
	return nullptr;
}