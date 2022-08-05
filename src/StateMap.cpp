#include <tuple>

#include "StateLevel.hpp"
#include "Time.hpp"
#include "Surface.hpp"
#include "Colour.hpp"
#include "StateMenu.hpp"

#include "StateMap.hpp"

StateMapRequirements::StateMapRequirements() :
	StateRequirements(StateKey::STATE_MAP)
{
}

StateMapRequirements::~StateMapRequirements() = default;

StateMap::StateMap(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil) :
	State(StateKey::STATE_MAP),
	ActionPressListener(inputManager),
	engine(engine),
	inputManager(inputManager),
	spriteManager(spriteManager),
	timeManager(timeManager),
	fontUtil(fontUtil),
	mapSurface(engine, MAP_TOTAL_WIDTH, MAP_TOTAL_HEIGHT),
	playerSurface(engine, "player_16_32.png"),
	playerShadow(engine, "player_shadow_14x8.png"),
	countryExterior(engine, "countries/country_normal_exterior_9x29.png"),
	labelSurface(engine, 150, 13),
	playerLabelSurface(engine, 160, 13),
	scoreLabelSurface(engine, 200, 24),
	mapPlayer(inputManager, playerSurface.getWidth(), playerSurface.getHeight(), playerSurface.getWidth() / 2, playerSurface.getHeight() - 1, 0, 0, MAP_TOTAL_WIDTH, MAP_TOTAL_HEIGHT)
{
	redraw();
}

StateMap::~StateMap() = default;

void StateMap::initialize(std::unique_ptr<StateRequirements> baseRequirements)
{
	StateMapRequirements requirements = dynamic_cast<StateMapRequirements&>(*baseRequirements);

	saveGame = requirements.saveGame;

	playerLabelSurface.drawFillTransparent(Colour(Colour::BLACK));
	playerLabelSurface.drawString((int) (0 + saveGame.playerName.length()), 3, saveGame.playerName, fontUtil.getFont(FontSize::SMALL), Colour::WHITE);

	for (auto& country : countries) {
		if (country.getLevel() < saveGame.level) {
			country.setComplete(true);
			country.updateSelected(false);
		} else {
			country.setComplete(false);
			country.updateSelected(false);
		}
	}

	mapPlayer.reset(401 + 25, 323 + 6);

	this->bindActionPressListener();
}

void StateMap::deinitialize()
{
	this->unbindActionPressListener();
}

void StateMap::tick()
{
	mapPlayer.tick();

	bool shouldRedraw = false, foundSelected = false;
	for (auto& country : countries) {
		if (country.collides(mapPlayer.getPlayerX(), mapPlayer.getPlayerY() + mapPlayer.getPlayerH() / 2)) {
			if (country.updateSelected(true)) {
				shouldRedraw = true;
			}
			if (!foundSelected && country.getState() == CountryState::SELECTED) {
				foundSelected = true;
				currentCountry = &country;
			}
		} else {
			if (country.updateSelected(false)) {
				shouldRedraw = true;
			}
		}
	}

	if (!foundSelected)
		currentCountry = nullptr;

	if (shouldRedraw)
		redraw();
}

void StateMap::render(Surface& viewSurface)
{
	auto offsetXY = getOffsetXY();
	viewSurface.drawRectangleFrom(mapSurface, 0, 0, VIEW_WIDTH, VIEW_HEIGHT, get<0>(offsetXY), get<1>(offsetXY));

	int mapPlayerX = mapPlayer.getPlayerX();
	int mapPlayerY = mapPlayer.getPlayerY();
	auto playerDrawOffset = getOffsetXY();

	viewSurface.drawRectangleFrom(playerShadow, mapPlayerX - std::get<0>(playerDrawOffset) + 1 - (playerSurface.getWidth() / 2), mapPlayerY - std::get<1>(playerDrawOffset) + 20 - (playerSurface.getWidth() / 2), playerShadow.getWidth(), playerShadow.getHeight());
	viewSurface.drawRectangleFrom(playerSurface, mapPlayerX - std::get<0>(playerDrawOffset) - (playerSurface.getWidth() / 2), mapPlayerY - std::get<1>(playerDrawOffset) - (playerSurface.getHeight() / 2), playerSurface.getWidth(), playerSurface.getHeight());

	if (currentCountry) {
		int posX = currentCountry->getWorldX() + currentCountry->getCapitalX();
		int posY = currentCountry->getWorldY() + currentCountry->getCapitalY();
		int labelWidth = (int) currentCountry->getName().length() * 10;
		int labelHeight = 13;
		viewSurface.drawRectangleFrom(labelSurface, (posX - labelWidth / 2) - std::get<0>(playerDrawOffset), (posY - 30) - std::get<1>(playerDrawOffset), labelWidth, labelHeight);
	}

	if (true) {
		int posX = mapPlayerX;
		int posY = mapPlayerY;
		int labelWidth = (int) saveGame.playerName.length() * 10;
		int labelHeight = 13;
		viewSurface.drawRectangleFrom(playerLabelSurface, (posX - labelWidth / 2) - std::get<0>(playerDrawOffset), (posY - 40) - std::get<1>(playerDrawOffset), labelWidth, labelHeight);
	}

	viewSurface.drawRectangleFrom(scoreLabelSurface, 4, 4, 6 + ((int) scoreLabelText.str().length() * 16), 6 + 16);
}

std::unique_ptr<StateRequirements> StateMap::getNextStateRequirements()
{
	return std::move(nextStateRequirements);
}

void StateMap::onActionPress(Press press, Action action)
{
	if (press == Press::UP && action == Action::EXIT) {
		auto stateMenuRequirements = std::make_unique<StateMenuRequirements>();
		nextStateRequirements = std::move(stateMenuRequirements);
		saveGameToFile(saveGame);
		setFinished(true);
		return;
	} else if (press == Press::UP && (action == Action::JUMP || action == Action::ENTER || action == Action::USE)) {
		for (auto& country : countries) {
			if (&country == currentCountry && !country.getComplete() && saveGame.level == country.getLevel()) {
				country.setComplete(true); // not neeeded
				saveGame.level++;
				redraw(); // not needed

				auto stateLevelRequirements = std::make_unique<StateLevelRequirements>();
				stateLevelRequirements->saveGame = saveGame;
				stateLevelRequirements->id = country.getLevel();
				stateLevelRequirements->levelCountry = country.getName();
				nextStateRequirements = std::move(stateLevelRequirements);
				setFinished(true);

				return;
			}
		}
	}

}

void StateMap::redraw()
{
	mapSurface.drawFill(Colour(0x8AB4F8));

	mapSurface.drawRectangleFrom(countryExterior, 9, 29, countryExterior.getWidth(), countryExterior.getHeight());

	for (auto& country : countries)
		if (!country.getComplete() && country.getState() == CountryState::NORMAL)
			mapSurface.drawRectangleFrom(country.getSurface(), country.getWorldX(), country.getWorldY(), country.getWidth(), country.getHeight());

	for (auto& country : countries)
		if (!country.getComplete() && country.getState() == CountryState::SELECTED)
			mapSurface.drawRectangleFrom(country.getSurface(), country.getWorldX(), country.getWorldY(), country.getWidth(), country.getHeight());

	for (auto& country : countries)
		if (country.getComplete())
			mapSurface.drawRectangleFrom(country.getSurface(), country.getWorldX(), country.getWorldY(), country.getWidth(), country.getHeight());

	mapSurface.drawString(419, 347, "1.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(362, 362, "2.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(373, 400, "3.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(349, 436, "4.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(338, 423, "5.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(323, 391, "6.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(312, 344, "7.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(375, 312, "8.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(381, 266, "9.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(356, 227, "10.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(376, 209, "11.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(375, 186, "12.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(304, 281, "13.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(281, 304, "14.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(274, 342, "15.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(225, 358, "16.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(220, 338, "17.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(143, 356, "18.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(63, 415, "19.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(57, 366, "20.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(151, 249, "21.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(106, 231, "22.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(202, 283, "23.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(217, 269, "24.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(226, 299, "25.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(253, 220, "26.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(286, 187, "27.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(353, 158, "28.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(253, 148, "29.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);
	mapSurface.drawString(116, 54, "30.", fontUtil.getFont(FontSize::SMALL), Colour::BLACK);

	scoreLabelSurface.drawFillTransparent(Colour(0x66000000));
	scoreLabelText.str(std::string());
	scoreLabelText << "Score:" << saveGame.score;
	scoreLabelSurface.drawString(4, 4, scoreLabelText.str(), fontUtil.getFont(FontSize::MEDIUM), Colour::WHITE);

	if (currentCountry) {
		int posX = currentCountry->getWorldX() + currentCountry->getCapitalX();
		int posY = currentCountry->getWorldY() + currentCountry->getCapitalY();

		mapSurface.drawOval(posX - 2, posY - 2, 4, 4, Colour::RED);

		labelSurface.drawFillTransparent(Colour(0x66000000));

		std::string countryName = currentCountry->getName();
		labelSurface.drawString((int) (0 + countryName.length()), 3, countryName, fontUtil.getFont(FontSize::SMALL), Colour::WHITE);
	}
}

std::tuple<int, int> StateMap::getOffsetXY() const
{
	int offsetX, offsetY;

	offsetX = mapPlayer.getPlayerX() - (VIEW_WIDTH / 2) + (mapPlayer.getPlayerW() / 2); // centre offset on player
	offsetY = mapPlayer.getPlayerY() - (VIEW_HEIGHT / 2) + (mapPlayer.getPlayerH() / 2);

	offsetX = offsetX < 0 ? 0 : offsetX; // bound to top & left
	offsetY = offsetY < 0 ? 0 : offsetY;

	offsetX = offsetX + VIEW_WIDTH >= MAP_TOTAL_WIDTH ? MAP_TOTAL_WIDTH - VIEW_WIDTH : offsetX; // bound to right and bottom
	offsetY = offsetY + VIEW_HEIGHT >= MAP_TOTAL_HEIGHT ? MAP_TOTAL_HEIGHT - VIEW_HEIGHT : offsetY;

	return std::make_tuple(offsetX, offsetY);
}