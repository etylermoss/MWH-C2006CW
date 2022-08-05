#ifndef __H_STATE_MANAGER__
#define __H_STATE_MANAGER__

#include <unordered_map>
#include <memory>

#include "lib/header.h"

#include "Tick.hpp"
#include "Render.hpp"
#include "Surface.hpp"
#include "InputManager.hpp"
#include "SpriteManager.hpp"
#include "TimeManager.hpp"
#include "State.hpp"
#include "Time.hpp"

class StateManager : public Tick, public Render
{
public:
	explicit StateManager(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil);
	~StateManager() override;

	void tick() override;
	void render(Surface& viewSurface) override;
	[[nodiscard]] bool getShouldExit() const;
	
private:
	BaseEngine* engine;
	InputManager& inputManager;
	SpriteManager& spriteManager;
	TimeManager& timeManager;
	FontUtil& fontUtil;
	std::unique_ptr<State> activeState;
	std::unordered_map<enum StateKey, std::unique_ptr<State>> inactiveStates;

	void changeState(std::unique_ptr<StateRequirements> requirements);
};

BaseEngine* StateManager::engine = nullptr;

#endif
