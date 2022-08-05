#include "lib/header.h"
#include "lib/BaseEngine.h"

#include "InputManager.hpp"

InputManager::InputManager(BaseEngine& engine, FilterPoints& filterPoints) :
	engine(engine),
	filterPoints(filterPoints),
	keyMap{
		{SDLK_LEFT,      Action::MOVE_LEFT},
		{SDLK_RIGHT,     Action::MOVE_RIGHT},
		{SDLK_UP,        Action::MOVE_UP},
		{SDLK_DOWN,      Action::MOVE_DOWN},
		{SDLK_a,         Action::MOVE_LEFT},
		{SDLK_d,         Action::MOVE_RIGHT},
		{SDLK_w,         Action::MOVE_UP},
		{SDLK_s,         Action::MOVE_DOWN},
		{SDLK_SPACE,     Action::JUMP},
		{SDLK_LCTRL,     Action::CROUCH},
		{SDLK_LSHIFT,    Action::RUN},
		{SDLK_e,         Action::USE},
		{SDLK_RETURN,    Action::ENTER},
		{SDLK_BACKSPACE, Action::UNDO},
		{SDLK_ESCAPE,    Action::EXIT},
	}
{
}

InputManager::~InputManager() = default;

void InputManager::mousePress(Press press, int iButton, int x, int y)
{
	MouseButton mouseButton = iButtonToMouseButton(iButton);

	for (const FuncMousePress& funcOnMousePress : onMousePressList)
		funcOnMousePress(press, mouseButton, x, y);
}

void InputManager::keyPress(Press press, SDL_KeyCode iKeyCode)
{
	char key = keyCodeToChar(iKeyCode);
	if (key)
		for (const FuncKeyPress& funcOnKeyPress : onKeyPressList)
			funcOnKeyPress(press, key);

	auto action = keyMap.find(iKeyCode);
	if (action == keyMap.end())
		return;

	for (const FuncActionPress& funcOnActionPress : onActionPressList)
		funcOnActionPress(press, action->second);
}

void InputManager::mouseMoved(int x, int y)
{
	for (const FuncMouseMove& funcOnMouseMove : onMouseMoveList)
		funcOnMouseMove(x, y);
}

void InputManager::mouseWheel(int x, int y, int which, int timestamp)
{
	if (which == SDL_TOUCH_MOUSEID)
		return;

	for (const FuncMouseWheel& funcOnMouseWheel : onMouseWheelList)
		funcOnMouseWheel(x, y, Time(timestamp));
}

Press InputManager::getActionPressed(Action action) const
{
	for (const auto& keyMapVal : keyMap)
		if (keyMapVal.second == action && engine.isKeyPressed(keyMapVal.first))
			return Press::DOWN;

	return Press::UP;
}

MousePosition InputManager::getMousePosition() const
{
	return std::make_tuple(engine.getCurrentMouseX(), engine.getCurrentMouseY());
}

MouseButtonStates InputManager::getMouseButtonStates() const
{
	int states = engine.getCurrentButtonStates();
	return std::make_tuple(
		((states & SDL_BUTTON_LMASK) != 0) ? Press::DOWN : Press::UP,
		((states & SDL_BUTTON_RMASK) != 0) ? Press::DOWN : Press::UP,
		((states & SDL_BUTTON_MMASK) != 0) ? Press::DOWN : Press::UP
	);
}

std::list<FuncMousePress>::iterator InputManager::bindOnMousePress(const FuncMousePress& onMousePress)
{
	return onMousePressList.insert(onMousePressList.end(), onMousePress);
}

std::list<FuncActionPress>::iterator InputManager::bindOnActionPress(const FuncActionPress& onActionPress)
{
	return onActionPressList.insert(onActionPressList.end(), onActionPress);
}

std::list<FuncKeyPress>::iterator InputManager::bindOnKeyPress(const FuncKeyPress& onKeyPress)
{
	return onKeyPressList.insert(onKeyPressList.end(), onKeyPress);
}

std::list<FuncMouseMove>::iterator InputManager::bindOnMouseMove(const FuncMouseMove& onMouseMove)
{
	return onMouseMoveList.insert(onMouseMoveList.end(), onMouseMove);
}

std::list<FuncMouseWheel>::iterator InputManager::bindOnMouseWheel(const FuncMouseWheel& onMouseWheel)
{
	return onMouseWheelList.insert(onMouseWheelList.end(), onMouseWheel);
}

void InputManager::unbindOnMousePress(std::list<FuncMousePress>::iterator onMousePressIterator)
{
	onMousePressList.erase(onMousePressIterator);
}

void InputManager::unbindOnActionPress(std::list<FuncActionPress>::iterator onActionPressIterator)
{
	onActionPressList.erase(onActionPressIterator);
}

void InputManager::unbindOnKeyPress(std::list<FuncKeyPress>::iterator onKeyPressIterator)
{
	onKeyPressList.erase(onKeyPressIterator);
}

void InputManager::unbindOnMouseMove(std::list<FuncMouseMove>::iterator onMouseMoveIterator)
{
	onMouseMoveList.erase(onMouseMoveIterator);
}

void InputManager::unbindOnMouseWheel(std::list<FuncMouseWheel>::iterator onMouseWheelIterator)
{
	onMouseWheelList.erase(onMouseWheelIterator);
}

/* see: https://wiki.libsdl.org/SDLKeycodeLookup */
char InputManager::keyCodeToChar(SDL_KeyCode iKeyCode)
{
	char key = 0;

	if (iKeyCode < 33 || iKeyCode > 122)
		return key;

	if (engine.isKeyPressed(SDLK_LSHIFT) || engine.isKeyPressed(SDLK_RSHIFT) && iKeyCode >= 'a' && iKeyCode <= 'z')
		key = static_cast<char>(iKeyCode) - ('a' - 'A');
	else
		key = static_cast<char>(iKeyCode);

	return key;
}

MouseButton InputManager::iButtonToMouseButton(int iButton)
{
	switch (iButton) {
	case SDL_BUTTON_LEFT:  return MouseButton::LEFT;
	case SDL_BUTTON_RIGHT: return MouseButton::RIGHT;
	default:               return MouseButton::MIDDLE;
	}
}

