#ifndef __H_INPUT_MANAGER__
#define __H_INPUT_MANAGER__

#include <functional>
#include <list>
#include <unordered_map>
#include <tuple>

#include "lib/header.h"
#include "lib/BaseEngine.h"
#include "lib/FilterPoints.h"

#include "Time.hpp"

enum class Press
{
	DOWN,
	UP,
};

enum class MouseButton
{
	LEFT,
	RIGHT,
	MIDDLE,
};

enum class Action
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	JUMP,
	CROUCH,
	RUN,
	USE,
	ENTER,
	UNDO,
	EXIT,
};

typedef std::function<void(Press press, MouseButton mouseButton, int x, int y)> FuncMousePress;
typedef std::function<void(Press press, Action action)> FuncActionPress;
typedef std::function<void(Press press, char key)> FuncKeyPress;
typedef std::function<void(int x, int y)> FuncMouseMove;
typedef std::function<void(int x, int y, Time timeCurr)> FuncMouseWheel;

typedef std::tuple<int, int> MousePosition; /* x, y */
typedef std::tuple<Press, Press, Press> MouseButtonStates; /* left, right, middle */

class InputManager
{
public:
	explicit InputManager(BaseEngine& engine, FilterPoints& pixelFilter);
	~InputManager();

	void mousePress(Press press, int iButton, int x, int y);
	void keyPress(Press press, SDL_KeyCode iKeyCode);
	void mouseMoved(int x, int y);
	void mouseWheel(int x, int y, int which, int timestamp);

	[[nodiscard]] Press getActionPressed(Action action) const;
	[[nodiscard]] MousePosition getMousePosition() const;
	[[nodiscard]] MouseButtonStates getMouseButtonStates() const;

	std::list<FuncMousePress>::iterator bindOnMousePress(const FuncMousePress& onMousePress);
	std::list<FuncActionPress>::iterator bindOnActionPress(const FuncActionPress& onActionPress);
	std::list<FuncKeyPress>::iterator bindOnKeyPress(const FuncKeyPress& onKeyPress);
	std::list<FuncMouseMove>::iterator bindOnMouseMove(const FuncMouseMove & onMouseMove);
	std::list<FuncMouseWheel>::iterator bindOnMouseWheel(const FuncMouseWheel& onMouseWheel);
	void unbindOnMousePress(std::list<FuncMousePress>::iterator onMousePressIterator);
	void unbindOnActionPress(std::list<FuncActionPress>::iterator onActionPressIterator);
	void unbindOnKeyPress(std::list<FuncKeyPress>::iterator onKeyPressIterator);
	void unbindOnMouseMove(std::list<FuncMouseMove>::iterator onMouseMoveIterator);
	void unbindOnMouseWheel(std::list<FuncMouseWheel>::iterator onMouseWheelIterator);

	char keyCodeToChar(SDL_KeyCode iKeyCode);

private:
	BaseEngine& engine;
	FilterPoints& filterPoints;

	std::unordered_map<SDL_KeyCode, Action> keyMap;
	std::list<FuncMousePress> onMousePressList;
	std::list<FuncActionPress> onActionPressList;
	std::list<FuncKeyPress> onKeyPressList;
	std::list<FuncMouseMove> onMouseMoveList;
	std::list<FuncMouseWheel> onMouseWheelList;

	static MouseButton iButtonToMouseButton(int iButton);
};

#endif
