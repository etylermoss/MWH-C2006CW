#ifndef __H_STATE_EXIT__
#define __H_STATE_EXIT__

#include <string>

#include "State.hpp"
#include "StateRequirements.hpp"
#include "InputManager.hpp"
#include "SpriteManager.hpp"
#include "TimeManager.hpp"
#include "FontUtil.hpp"
#include "Time.hpp"
#include "TextScroll.hpp"
#include "Surface.hpp"

struct StateExitRequirements;
class StateExit;
class TextScroll;

struct StateExitRequirements : public StateRequirements
{
public:
	StateExitRequirements();
	~StateExitRequirements() override;
};

class StateExit : public State
{
public:
	explicit StateExit(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil);
	~StateExit() override;

	void initialize(std::unique_ptr<StateRequirements> baseRequirements) override;
	void deinitialize() override;
	void tick() override;
	void render(Surface& viewSurface) override;
	std::unique_ptr<StateRequirements> getNextStateRequirements() override;

private:
	InputManager& inputManager;
	SpriteManager& spriteManager;
	TimeManager& timeManager;
	FontUtil& fontUtil;
	Surface foreground, background;
	TextScroll textScroll;
};

#endif