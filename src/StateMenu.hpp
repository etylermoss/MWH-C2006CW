#ifndef __H_STATE_MENU__
#define __H_STATE_MENU__

#include "State.hpp"
#include "StateRequirements.hpp"
#include "Engine.hpp"
#include "Time.hpp"
#include "InputManager.hpp"
#include "FontUtil.hpp"
#include "SpriteManager.hpp"
#include "TimeManager.hpp"
#include "InputListeners.hpp"
#include "Surface.hpp"
#include "MenuButton.hpp"
#include "MenuInput.hpp"

// TODO: DisplayAble objects in background, simple circles that grow from nothing, bounce around until hitting each other, then despawn with animation
// do last, will be annoying. Maybe make the background grid respond to mouse movement (just simple shifting around

struct StateMenuRequirements : public StateRequirements
{
public:
	StateMenuRequirements();
	~StateMenuRequirements() override;
};

class StateMenu : public State, MousePressListener, MouseMoveListener, ActionPressListener, KeyPressListener
{
public:
	explicit StateMenu(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil);
	~StateMenu() override;

	void initialize(std::unique_ptr<StateRequirements> baseRequirements) override;
	void deinitialize() override;
	void tick() override;
	void render(Surface& viewSurface) override;
	std::unique_ptr<StateRequirements> getNextStateRequirements() override;

	void onMousePress(Press press, MouseButton mouseButton, int x, int y) override;
	void onMouseMove(int x, int y) override;
	void onActionPress(Press press, Action action) override;
	void onKeyPress(Press press, char key) override;

private:
	InputManager& inputManager;
	SpriteManager& spriteManager;
	TimeManager& timeManager;
	FontUtil& fontUtil;
	std::unique_ptr<StateRequirements> nextStateRequirements;

	Surface background;
	Surface flag;
	MenuButton startButton, loadButton, exitButton;
	MenuInput playerNameInput;

	void newGame();
	void loadGame();
};

#endif
