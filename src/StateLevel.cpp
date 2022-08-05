#include "StateExit.hpp"
#include "Time.hpp"
#include "Colour.hpp"
#include "MyTileManager.hpp"
#include "StateMap.hpp"

#include <sstream>
#include "lib/UtilCollisionDetection.h"

#include "StateLevel.hpp"

StateLevelRequirements::StateLevelRequirements() :
	StateRequirements(StateKey::STATE_LEVEL)
{
}

StateLevelRequirements::~StateLevelRequirements() = default;

StateLevel::StateLevel(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil) :
	State(StateKey::STATE_LEVEL),
	ActionPressListener(inputManager),
	engine(engine),
	inputManager(inputManager),
	spriteManager(spriteManager),
	timeManager(timeManager),
	fontUtil(fontUtil),
	paused(false),
	levelSurface(engine, VIEW_WIDTH, VIEW_HEIGHT),
	playerSprite(engine, "player_16_32.png"),
	level(),
	myTileManager(engine),
	player(inputManager)
{
	levelSurface.drawFill(Colour(0x8AB4F8));
}

StateLevel::~StateLevel() = default;

void StateLevel::initialize(std::unique_ptr<StateRequirements> baseRequirements)
{
	StateLevelRequirements requirements = dynamic_cast<StateLevelRequirements&>(*baseRequirements);

	level = Level(requirements.id, requirements.levelCountry);
	myTileManager.loadLevel(level);
	paused = false;
	livesRemaining = 3;
	saveGame = requirements.saveGame;
	timeStart = timeManager.getTime();

	using namespace std;
	auto levelSpawn = level.getSpawn();
	auto levelSize = level.getSize();

	player.setLevel(&level);

	player.reset(get<0>(levelSpawn), get<1>(levelSpawn), 0.0f, 0.0f, 0, 0, get<0>(levelSize) * TILE_SIZE, get<1>(levelSize) * TILE_SIZE, playerSprite.getWidth(), playerSprite.getHeight(), 7, 25);

	this->bindActionPressListener();
}

void StateLevel::deinitialize()
{
	this->unbindActionPressListener();
}

void StateLevel::tick()
{
	if (paused)
		return;

	player.tick();

	auto levelEnd = level.getEnd();
	int levelEndX = get<0>(levelEnd) * TILE_SIZE;
	int levelEndY = get<1>(levelEnd) * TILE_SIZE;
	if (CollisionDetection::checkRectangles(player.getPlayerX(), player.getPlayerX() + player.getPlayerW(), player.getPlayerY(), player.getPlayerY() + player.getPlayerH(), levelEndX, levelEndX + 16, levelEndY, levelEndY + 32)) {
		auto req = std::make_unique<StateMapRequirements>();
		saveGame.level = level.getID() + 1;
		saveGame.score += (int) (100l / timeManager.getTime().differenceFrom(timeStart).getSeconds());
		req->saveGame = saveGame;
		saveGameToFile(saveGame);
		nextStateRequirements = std::move(req);
		setFinished(true);
		return;
	}

	// check if completed the level
	// update barrel positions, physics
}

void StateLevel::render(Surface& viewSurface)
{
	viewSurface.drawFromSurface(levelSurface);

	myTileManager.render(viewSurface);

	auto levelEnd = level.getEnd(); get<0>(levelEnd) *= TILE_SIZE; get<1>(levelEnd) *= TILE_SIZE;
	auto levelEndSize = level.getEndSize();
	viewSurface.drawRectangle(get<0>(levelEnd), get<1>(levelEnd), get<0>(levelEndSize), get<1>(levelEndSize), Colour(0x181621));
	viewSurface.drawRectangle(get<0>(levelEnd) + 3, get<1>(levelEnd) + 3, get<0>(levelEndSize) - 3 - 3, get<1>(levelEndSize) - 3 - 3, Colour(0x33295F));
	viewSurface.drawRectangle(get<0>(levelEnd) + 5, get<1>(levelEnd) + 5, get<0>(levelEndSize) - 5 - 5, get<1>(levelEndSize) - 5 - 5, Colour(0x3C2E76));
	viewSurface.drawRectangle(get<0>(levelEnd) + 6, get<1>(levelEnd) + 6, get<0>(levelEndSize) - 6 - 6, get<1>(levelEndSize) - 6 - 6, Colour(0x463494));

	viewSurface.drawRectangleFrom(playerSprite, player.getPlayerX(), player.getPlayerY(), player.getPlayerW(), player.getPlayerH());


	std::stringstream levelLabel;
	levelLabel << "Level " << level.getID() << ": " << level.getTitle();
	viewSurface.drawString(4, 4, levelLabel.str(), fontUtil.getFont(FontSize::SMALL), Colour::BLACK);



}

std::unique_ptr<StateRequirements> StateLevel::getNextStateRequirements()
{
	return std::move(nextStateRequirements);
}

void StateLevel::onActionPress(Press press, Action action)
{

}
