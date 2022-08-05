#include "Constants.hpp"
#include "Time.hpp"
#include "StateMap.hpp"
#include "StateRequirements.hpp"
#include "InputListeners.hpp"
#include "Colour.hpp"
#include "StateExit.hpp"
#include "SaveGame.hpp"

#include "StateMenu.hpp"

StateMenuRequirements::StateMenuRequirements() :
	StateRequirements(StateKey::STATE_MENU)
{
}

StateMenuRequirements::~StateMenuRequirements() = default;

StateMenu::StateMenu(BaseEngine* engine, InputManager& inputManager, SpriteManager& spriteManager, TimeManager& timeManager, FontUtil& fontUtil) :
	State(StateKey::STATE_MENU),
	MousePressListener(inputManager),
	MouseMoveListener(inputManager),
	ActionPressListener(inputManager),
	KeyPressListener(inputManager),
	inputManager(inputManager),
	fontUtil(fontUtil),
	spriteManager(spriteManager),
	timeManager(timeManager),
	background(engine, VIEW_WIDTH, VIEW_HEIGHT),
	flag(engine, "flag_160x120.png"),
	startButton(engine, fontUtil, "New Game", 188, 150 + 16),
	loadButton(engine, fontUtil, "Load Game", 180, 150 + 48),
	exitButton(engine, fontUtil, "Quit", 220, 150 + 80),
	playerNameInput(engine, fontUtil, (512 / 2) - (264 / 2), 150 + 16, false)
{
	for (int i = 0; i < MAP_WIDTH; ++i) {
		for (int j = 0; j < MAP_HEIGHT; ++j) {
			Colour colour = (i + j) % 2 ? Colour::BLACK : Colour(0x111111);
			background.drawOval(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, colour);
		}
	}

	background.drawRectangleFrom(flag, (VIEW_WIDTH / 2) - (flag.getWidth() / 2), 20, flag.getWidth(), flag.getHeight());
	background.drawString(97, 70, "Moldovan Wine Hopper", fontUtil.getFont(FontSize::MEDIUM), Colour::WHITE);
}

StateMenu::~StateMenu() = default;

void StateMenu::initialize(std::unique_ptr<StateRequirements> baseRequirements)
{
	StateMenuRequirements requirements = dynamic_cast<StateMenuRequirements&>(*baseRequirements);
	startButton.setEnabled(true);
	loadButton.setEnabled(true);
	exitButton.setEnabled(true);
	playerNameInput.setEnabled(false);

	this->bindMousePressListener();
	this->bindMouseMoveListener();
	this->bindActionPressListener();
	this->bindKeyPressListener();
}

void StateMenu::deinitialize()
{
	this->unbindMousePressListener();
	this->unbindMouseMoveListener();
	this->unbindActionPressListener();
	this->unbindKeyPressListener();
}

void StateMenu::tick()
{
}

void StateMenu::render(Surface& viewSurface)
{
	viewSurface.drawFromSurface(background);
	startButton.render(viewSurface);
	loadButton.render(viewSurface);
	exitButton.render(viewSurface);
	playerNameInput.render(viewSurface);
}

std::unique_ptr<StateRequirements> StateMenu::getNextStateRequirements()
{
	return std::move(nextStateRequirements);
}

void StateMenu::onMousePress(Press press, MouseButton mouseButton, int x, int y)
{
	if (startButton.clickButton(press, mouseButton, x, y)) {
		startButton.setEnabled(false);
		loadButton.setEnabled(false);
		exitButton.setEnabled(false);
		playerNameInput.setEnabled(true);
	}

	if (loadButton.clickButton(press, mouseButton, x, y)) {
		this->loadGame();
	}

	if (exitButton.clickButton(press, mouseButton, x, y)) {
		nextStateRequirements = std::make_unique<StateExitRequirements>();
		this->setFinished(true);
	}
}

void StateMenu::onMouseMove(int x, int y)
{
	startButton.mouseMoved(x, y);
	loadButton.mouseMoved(x, y);
	exitButton.mouseMoved(x, y);
}

void StateMenu::onActionPress(Press press, Action action)
{
	if (action == Action::EXIT && press == Press::UP && exitButton.getEnabled()) {
		nextStateRequirements = std::make_unique<StateExitRequirements>();
		this->setFinished(true);
	} else if (action == Action::EXIT && press == Press::UP && playerNameInput.getEnabled()) {
		startButton.setEnabled(true);
		loadButton.setEnabled(true);
		exitButton.setEnabled(true);
		playerNameInput.setEnabled(false);
	} else if (action == Action::ENTER && press == Press::UP && playerNameInput.getEnabled()) {
		if (playerNameInput.inputSatisfied())
			this->newGame();
	} else if (action == Action::UNDO && press == Press::UP && playerNameInput.getEnabled()) {
		playerNameInput.pressUndoKey();
	}
}

void StateMenu::onKeyPress(Press press, char key)
{
	if (press == Press::UP)
		playerNameInput.pressKey(key);
}

void StateMenu::newGame()
{
	std::unique_ptr<StateMapRequirements> mapRequirements = std::make_unique<StateMapRequirements>();

	mapRequirements->saveGame.playerName = playerNameInput.getInput();
	mapRequirements->saveGame.level = 1;
	mapRequirements->saveGame.score = 0;

	saveGameToFile(mapRequirements->saveGame);

	nextStateRequirements = std::move(mapRequirements);
	this->setFinished(true);
}

void StateMenu::loadGame()
{
	std::optional<SaveGame> saveGame = loadGameFromFile();

	if (!saveGame)
		return;

	std::unique_ptr<StateMapRequirements> mapRequirements = std::make_unique<StateMapRequirements>();

	mapRequirements->saveGame = saveGame.value();

	nextStateRequirements = std::move(mapRequirements);
	this->setFinished(true);
}




