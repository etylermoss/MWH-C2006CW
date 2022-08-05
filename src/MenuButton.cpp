#include "InputManager.hpp"

#include "MenuButton.hpp"

MenuButton::MenuButton(BaseEngine *engine, FontUtil &fontUtil, const std::string &label, int posX, int posY, bool enabled) :
	Render(),
	engine(engine),
	fontUtil(fontUtil),
	label(label),
	posX(posX),
	posY(posY),
	width((FontUtil::sizeToFontPX(fontSize) * (int) label.length()) + 8),
	height(FontUtil::sizeToFontPX(fontSize) + 8),
	surface(engine, width, height),
	enabled(enabled),
	hover(false),
	active(false)
{
	if (label.empty())
		throw std::invalid_argument("MenuButton::MenuButton(), label length must be > 0");

	redraw();
}

MenuButton::~MenuButton()
{
}

void MenuButton::render(Surface &targetSurface)
{
	if (!enabled)
		return;

	targetSurface.drawRectangleFrom(surface, posX, posY, width, height, 0, 0);
}

bool MenuButton::checkCollision(int x, int y) const
{
	return (x >= posX && y >= posY && x <= posX + width && y <= posY + height);
}

void MenuButton::mouseMoved(int x, int y)
{
	if (!enabled)
		return;

	const bool collides = checkCollision(x, y);

	if (hover && !collides) {
		hover = false;
		redraw();
	} else if (collides) {
		hover = true;
		redraw();
	}
}

bool MenuButton::clickButton(Press press, MouseButton mouseButton, int x, int y)
{
	const bool collides = enabled ? checkCollision(x, y) : false;

	if (collides && press == Press::DOWN && mouseButton == MouseButton::LEFT) {
		active = true;
		redraw();
	} else if (collides && press == Press::UP && mouseButton == MouseButton::LEFT && active) {
		active = false;
		redraw();
		return true;
	} else if (!collides && press == Press::UP && mouseButton == MouseButton::LEFT) {
		active = false;
		redraw();
	}

	return false;
}

void MenuButton::setEnabled(bool newEnabled)
{
	enabled = newEnabled;
	if (enabled)
		redraw();
}

bool MenuButton::getEnabled() const
{
	return enabled;
}

void MenuButton::redraw()
{
	if (!enabled)
		return;

	const Colour lineColour (active ? colourActive : hover ? colourHover : colourNoHover);
	const Colour textColour (active ? colourHover : hover ? colourHover : colourNoHover);

	/* background */
	surface.drawFill(colourBackground);

	/* button lines */
	surface.drawLine(0, 0, width - 1, 0, 2, lineColour); // top
	surface.drawLine(width - 1, 0, width - 1, height - 1, 2, lineColour); // right
	surface.drawLine(0, height - 1, width - 1, height - 1, 2, lineColour); // bottom
	surface.drawLine(0, 0, 0, height - 1, 2, lineColour); // left

	/* label */
	surface.drawString(5, 5, label, fontUtil.getFont(fontSize), textColour);
}
