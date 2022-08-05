#ifndef __H_STATE_MAP__
#define __H_STATE_MAP__

#include <string>

#include "InputManager.hpp"
#include "InputListeners.hpp"
#include "SpriteManager.hpp"
#include "TimeManager.hpp"
#include "FontUtil.hpp"
#include "State.hpp"
#include "StateRequirements.hpp"
#include "Time.hpp"
#include "Surface.hpp"
#include "SaveGame.hpp"
#include "MapCountry.hpp"
#include "MapPlayer.hpp"

struct StateMapRequirements : public StateRequirements
{
public:
	StateMapRequirements();
	~StateMapRequirements() override;

	SaveGame saveGame;
};

class StateMap : public State, private ActionPressListener
{
public:
	explicit StateMap(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil);
	~StateMap() override;

	void initialize(std::unique_ptr<StateRequirements> baseRequirements) override;
	void deinitialize() override;
	void tick() override;
	void render(Surface& viewSurface) override;
	std::unique_ptr<StateRequirements> getNextStateRequirements() override;

	void onActionPress(Press press, Action action) override;

private:
	BaseEngine* engine;
	InputManager& inputManager;
	FontUtil& fontUtil;
	SpriteManager& spriteManager;
	TimeManager& timeManager;
	Surface mapSurface, playerSurface, playerShadow, countryExterior, labelSurface, playerLabelSurface, scoreLabelSurface;
	SaveGame saveGame;
	MapPlayer mapPlayer;
	MapCountry* currentCountry = nullptr;
	std::stringstream scoreLabelText;
	std::array<MapCountry, 30> countries = {
			MapCountry(engine, 5, "Albania", "albania", 335, 404, 8, 18),
			MapCountry(engine, 15, "Austria", "austria", 245, 321, 51, 16),
			MapCountry(engine, 9, "Belarus", "belarus", 370, 224, 39, 30),
			MapCountry(engine, 23, "Belgium", "belgium", 194, 277, 19, 11),
			MapCountry(engine, 3, "Bulgaria", "bulgaria", 364, 379, 30, 21),
			MapCountry(engine, 14, "Czechoslovakia", "czechoslovakia", 271, 293, 48, 24),
			MapCountry(engine, 26, "Denmark", "denmark", 251, 203, 9, 22),
			MapCountry(engine, 12, "Estonia", "estonia", 366, 182, 21, 12),
			MapCountry(engine, 28, "Finland", "finland", 343, 49, 35, 100),
			MapCountry(engine, 18, "France", "france", 121, 278, 65, 35),
			MapCountry(engine, 25, "Deutschland", "germany", 219, 240, 55, 35),
			MapCountry(engine, 4, "Greece", "greece", 342, 409, 16, 30),
			MapCountry(engine, 7, "Hungary", "hungary", 306, 325, 32, 20),
			MapCountry(engine, 30, "Iceland", "iceland", 102, 32, 35, 37),
			MapCountry(engine, 22, "Ireland", "ireland", 94, 202, 34, 26),
			MapCountry(engine, 16, "Italy", "italy", 211, 343, 60, 65),
			MapCountry(engine, 11, "Latvia", "latvia", 350, 203, 38, 12),
			MapCountry(engine, 10, "Lithuania", "lithuania", 350, 221, 24, 16),
			MapCountry(engine, 1, "Moldova", "moldova", 401, 323, 26, 23),
			MapCountry(engine, 24, "The Netherlands", "netherlands", 205, 255, 13, 18),
			MapCountry(engine, 29, "Norway", "norway", 239, 34, 15, 158),
			MapCountry(engine, 13, "Poland", "poland", 293, 240, 50, 36),
			MapCountry(engine, 20, "Portugal", "portugal", 32, 355, 12, 38),
			MapCountry(engine, 2, "Romania", "romania", 348, 324, 47, 44),
			MapCountry(engine, 19, "Spain", "spain", 50, 341, 45, 55),
			MapCountry(engine, 27, "Sweden", "sweden", 282, 65, 34, 120),
			MapCountry(engine, 17, "Switzerland", "switzerland", 212, 331, 14, 9),
			MapCountry(engine, 21, "United Kingdom", "uk", 125, 169, 48, 97),
			MapCountry(engine, 8, "Ukraine", "ukraine", 360, 268, 103, 57),
			MapCountry(engine, 6, "Yugoslavia", "yugoslavia", 280, 349, 62, 34),
	};
	std::unique_ptr<StateRequirements> nextStateRequirements;

	void redraw();

	[[nodiscard]] std::tuple<int, int> getOffsetXY() const;
};

#endif
