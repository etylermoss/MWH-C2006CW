#include <memory>
#include <map>

#include "Tick.hpp"
#include "Render.hpp"
#include "StateExit.hpp"
#include "StateLevel.hpp"
#include "StateMap.hpp"
#include "StateMenu.hpp"

#include "StateManager.hpp"

StateManager::StateManager(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil) :
	Tick(),
	Render(),
	engine(engine),
	inputManager(inputManager),
	spriteManager(spriteManager),
	timeManager(timeManager),
	fontUtil(fontUtil),
	activeState(),
	inactiveStates()
{
	auto menuRequirements(std::make_unique<StateMenuRequirements>());

	this->changeState(std::move(menuRequirements));
}

StateManager::~StateManager() = default;

void StateManager::tick()
{
	activeState->tick();

	if (!activeState->getShouldExit() && activeState->getFinished()) {
		std::unique_ptr<StateRequirements> nextStateRequirements = activeState->getNextStateRequirements();

		activeState->deinitialize();

		this->changeState(std::move(nextStateRequirements));
	} else if (activeState->getShouldExit()) {
		activeState->deinitialize();
	}
}

void StateManager::render(Surface& viewSurface)
{
	activeState->render(viewSurface);
}

bool StateManager::getShouldExit() const
{
	return activeState->getShouldExit();
}

void StateManager::changeState(std::unique_ptr<StateRequirements> requirements)
{
	if (activeState)
		inactiveStates.emplace(activeState->getStateKey(), std::move(activeState));

	/* Set active state to the next state, constructing it if necessary, and removing the next state from the inactive states map */
	auto inactiveStateIterator = inactiveStates.find(requirements->getStateKey());
	if (inactiveStateIterator == inactiveStates.end()) {
		switch (requirements->getStateKey()) {
			case StateKey::STATE_EXIT:  activeState = std::make_unique<StateExit>(engine, inputManager, spriteManager, timeManager, fontUtil);  break;
			case StateKey::STATE_LEVEL: activeState = std::make_unique<StateLevel>(engine, inputManager, spriteManager, timeManager, fontUtil); break;
			case StateKey::STATE_MAP:   activeState = std::make_unique<StateMap>(engine, inputManager, spriteManager, timeManager, fontUtil);   break;
			case StateKey::STATE_MENU:  activeState = std::make_unique<StateMenu>(engine, inputManager, spriteManager, timeManager, fontUtil);  break;
		}
	} else {
		activeState = std::move(inactiveStateIterator->second);
		inactiveStates.erase(inactiveStateIterator);
	}

	activeState->setFinished(false);
	activeState->initialize(std::move(requirements));
}