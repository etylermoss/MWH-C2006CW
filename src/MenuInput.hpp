#ifndef __H_MENU_INPUT__
#define __H_MENU_INPUT__

#include <string>
#include <stdexcept>

#include "lib/BaseEngine.h"

#include "Render.hpp"
#include "Surface.hpp"
#include "FontUtil.hpp"

class MenuInput : public Render
{
public:
	MenuInput(BaseEngine* engine, FontUtil& fontUtil, int posX, int posY, bool enabled = true);

	void render(Surface& targetSurface) override;

	void setEnabled(bool newEnabled);
	[[nodiscard]] bool getEnabled() const;

	void pressKey(char key);
	void pressUndoKey();

	[[nodiscard]] bool inputSatisfied() const;
	[[nodiscard]] std::string getInput() const;

private:
	BaseEngine* engine;
	FontUtil& fontUtil;
	int minChars, maxChars;
	int posX, posY;
	int width, height;
	Surface surface;
	bool enabled;
	std::string input;

	const Colour colourBackground = Colour(Colour::BLACK);
	const Colour colourLine = Colour(Colour::WHITE);
	const Colour colourIncompleteText = Colour(0xA0A0A0);
	const Colour colourCompleteText = Colour(Colour::WHITE);
	static const enum FontSize fontSize = FontSize::MEDIUM;

	void redraw();
};

#endif
