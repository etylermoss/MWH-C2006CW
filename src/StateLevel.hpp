#ifndef __H_STATE_LEVEL__
#define __H_STATE_LEVEL__

#include <optional>
#include <vector>

#include "InputManager.hpp"
#include "SpriteManager.hpp"
#include "TimeManager.hpp"
#include "FontUtil.hpp"
#include "State.hpp"
#include "StateRequirements.hpp"
#include "InputListeners.hpp"
#include "Time.hpp"
#include "Surface.hpp"
#include "SaveGame.hpp"
#include "Level.hpp"
#include "MyTileManager.hpp"
#include "LevelPlayer.hpp"

struct StateLevelRequirements : public StateRequirements
{
public:
	StateLevelRequirements();
	~StateLevelRequirements() override;

	SaveGame saveGame;
	int id {};
	std::string levelCountry;
};

class StateLevel : public State, private ActionPressListener
{
public:
	explicit StateLevel(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil);
	~StateLevel() override;

	void initialize(std::unique_ptr<StateRequirements> baseRequirements) override;
	void deinitialize() override;
	void tick() override;
	void render(Surface& viewSurface) override;
	std::unique_ptr<StateRequirements> getNextStateRequirements() override;

	void onActionPress(Press press, Action action) override;

private:
	/* core objects*/
	BaseEngine* engine;
	InputManager& inputManager;
	SpriteManager& spriteManager;
	TimeManager& timeManager;
	FontUtil& fontUtil;
	std::unique_ptr<StateRequirements> nextStateRequirements;

	/* derived specific */
	Time timeStart;
	bool paused;
	MyTileManager myTileManager;
	Surface levelSurface, playerSprite;
	Level level;
	LevelPlayer player;
	SaveGame saveGame;
	int livesRemaining;
	int score;
	//std::vector<Barrel> barrels;
};

#endif
