#include "InputManager.hpp"

#include "MenuInput.hpp"

MenuInput::MenuInput(BaseEngine *engine, FontUtil &fontUtil, int posX, int posY, bool enabled) :
		Render(),
		engine(engine),
		fontUtil(fontUtil),
		minChars(4),
		maxChars(16),
		posX(posX),
		posY(posY),
		width((FontUtil::sizeToFontPX(fontSize) * (int) maxChars) + 8),
		height(FontUtil::sizeToFontPX(fontSize) + 12),
		surface(engine, width, height),
		enabled(enabled),
		input()
{
	redraw();
}

void MenuInput::render(Surface &targetSurface)
{
	if (!enabled)
		return;

	targetSurface.drawRectangleFrom(surface, posX, posY, width, height, 0, 0);
}

void MenuInput::setEnabled(bool newEnabled)
{
	enabled = newEnabled;
	if (enabled)
		redraw();
}

bool MenuInput::getEnabled() const
{
	return enabled;
}

void MenuInput::pressKey(char key)
{
	if (!enabled || input.length() >= maxChars)
		return;
	input.push_back(key);
	redraw();
}

void MenuInput::pressUndoKey()
{
	if (!enabled || input.length() < 1)
		return;
	input.resize(input.length() - 1);
	redraw();
}

bool MenuInput::inputSatisfied() const
{
	return input.length() >= minChars && input.length() <= maxChars;
}

std::string MenuInput::getInput() const
{
	return input;
}

void MenuInput::redraw()
{
	/* background */
	surface.drawFill(colourBackground);

	/* button lines */
	surface.drawLine(0, 0, width - 1, 0, 2, colourLine); // top
	surface.drawLine(width - 1, 0, width - 1, height - 1, 2, colourLine); // right
	surface.drawLine(0, height - 1, width - 1, height - 1, 2, colourLine); // bottom
	surface.drawLine(0, 0, 0, height - 1, 2, colourLine); // left

	/* underline */
	surface.drawString(5, 7, "________________", fontUtil.getFont(fontSize), inputSatisfied() ? colourCompleteText : colourIncompleteText);

	/* label */
	if (input.empty())
		surface.drawString(5, 5, "Enter name ...", fontUtil.getFont(fontSize), inputSatisfied() ? colourCompleteText : colourIncompleteText);
	else
		surface.drawString(5, 5, input, fontUtil.getFont(fontSize), inputSatisfied() ? colourCompleteText : colourIncompleteText);
}
