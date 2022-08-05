#include "InputListeners.hpp"

using namespace std::placeholders;

/* MousePressListener */

MousePressListener::MousePressListener(InputManager& inputManager) :
	inputManager(inputManager)
{
}

MousePressListener::~MousePressListener()
{
	if (onMousePressIterator)
		unbindMousePressListener();
}

void MousePressListener::bindMousePressListener()
{
	if (!onMousePressIterator)
		onMousePressIterator = inputManager.bindOnMousePress(std::bind(&MousePressListener::onMousePress, this, _1, _2, _3, _4));
}

void MousePressListener::unbindMousePressListener()
{
	if (onMousePressIterator) {
		inputManager.unbindOnMousePress(*onMousePressIterator);
		onMousePressIterator.reset();
	}
}

/* ActionPressListener */

ActionPressListener::ActionPressListener(InputManager& inputManager) :
	inputManager(inputManager)
{
}

ActionPressListener::~ActionPressListener()
{
	if (onActionPressIterator)
		unbindActionPressListener();
}

void ActionPressListener::bindActionPressListener()
{
	if (!onActionPressIterator)
		onActionPressIterator = inputManager.bindOnActionPress(std::bind(&ActionPressListener::onActionPress, this, _1, _2));
}

void ActionPressListener::unbindActionPressListener()
{
	if (onActionPressIterator) {
		inputManager.unbindOnActionPress(*onActionPressIterator);
		onActionPressIterator.reset();
	}
}

/* KeyPressListener */

KeyPressListener::KeyPressListener(InputManager& inputManager) :
		inputManager(inputManager)
{
}

KeyPressListener::~KeyPressListener()
{
	if (onKeyPressIterator)
		unbindKeyPressListener();
}

void KeyPressListener::bindKeyPressListener()
{
	if (!onKeyPressIterator)
		onKeyPressIterator = inputManager.bindOnKeyPress(std::bind(&KeyPressListener::onKeyPress, this, _1, _2));
}

void KeyPressListener::unbindKeyPressListener()
{
	if (onKeyPressIterator) {
		inputManager.unbindOnKeyPress(*onKeyPressIterator);
		onKeyPressIterator.reset();
	}
}

/* MouseMoveListener */

MouseMoveListener::MouseMoveListener(InputManager& inputManager) :
	inputManager(inputManager)
{
}

MouseMoveListener::~MouseMoveListener()
{
	if (onMouseMoveIterator)
		unbindMouseMoveListener();
}

void MouseMoveListener::bindMouseMoveListener()
{
	if (!onMouseMoveIterator)
		onMouseMoveIterator = inputManager.bindOnMouseMove(std::bind(&MouseMoveListener::onMouseMove, this, _1, _2));
}

void MouseMoveListener::unbindMouseMoveListener()
{
	if (onMouseMoveIterator) {
		inputManager.unbindOnMouseMove(*onMouseMoveIterator);
		onMouseMoveIterator.reset();
	}
}

/* MouseWheelListener */

MouseWheelListener::MouseWheelListener(InputManager& inputManager) :
	inputManager(inputManager)
{
}

MouseWheelListener::~MouseWheelListener()
{
	if (onMouseWheelIterator)
		unbindMouseWheelListener();
}

void MouseWheelListener::bindMouseWheelListener()
{
	if (!onMouseWheelIterator)
		onMouseWheelIterator = inputManager.bindOnMouseWheel(std::bind(&MouseWheelListener::onMouseWheel, this, _1, _2, _3));
}

void MouseWheelListener::unbindMouseWheelListener()
{
	if (onMouseWheelIterator) {
		inputManager.unbindOnMouseWheel(*onMouseWheelIterator);
		onMouseWheelIterator.reset();
	}
}
