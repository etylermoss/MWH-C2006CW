#ifndef __H_INPUT_LISTENER__
#define __H_INPUT_LISTENER__

#include <list>
#include <optional>

#include "InputManager.hpp"

class MousePressListener
{
protected:
	explicit MousePressListener(InputManager& inputManager);
	~MousePressListener();

	void bindMousePressListener();
	void unbindMousePressListener();

	virtual void onMousePress(Press press, MouseButton mouseButton, int x, int y) = 0;

private:
	InputManager& inputManager;
	std::optional<std::list<FuncMousePress>::iterator> onMousePressIterator;
};

class ActionPressListener
{
protected:
	explicit ActionPressListener(InputManager& inputManager);
	~ActionPressListener();

	void bindActionPressListener();
	void unbindActionPressListener();

	virtual void onActionPress(Press press, Action action) = 0;

private:
	InputManager& inputManager;
	std::optional<std::list<FuncActionPress>::iterator> onActionPressIterator;
};

class KeyPressListener
{
protected:
	explicit KeyPressListener(InputManager& inputManager);
	~KeyPressListener();

	void bindKeyPressListener();
	void unbindKeyPressListener();

	virtual void onKeyPress(Press press, char key) = 0;

private:
	InputManager& inputManager;
	std::optional<std::list<FuncKeyPress>::iterator> onKeyPressIterator;
};

class MouseMoveListener
{
protected:
	explicit MouseMoveListener(InputManager& inputManager);
	~MouseMoveListener();

	void bindMouseMoveListener();
	void unbindMouseMoveListener();

	virtual void onMouseMove(int x, int y) = 0;

private:
	InputManager& inputManager;
	std::optional<std::list<FuncMouseMove>::iterator> onMouseMoveIterator;
};

class MouseWheelListener
{
protected:
	explicit MouseWheelListener(InputManager& inputManager);
	~MouseWheelListener();

	void bindMouseWheelListener();
	void unbindMouseWheelListener();

	virtual void onMouseWheel(int x, int y, Time timeCurr) = 0;

private:
	InputManager& inputManager;
	std::optional<std::list<FuncMouseWheel>::iterator> onMouseWheelIterator;
};

#endif
