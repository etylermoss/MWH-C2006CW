#ifndef __H_MENU_BUTTON__
#define __H_MENU_BUTTON__

#include <string>
#include <stdexcept>

#include "lib/BaseEngine.h"
#include "lib/UtilCollisionDetection.h"

#include "Render.hpp"
#include "Surface.hpp"
#include "FontUtil.hpp"

class MenuButton : public Render
{
public:
	MenuButton(BaseEngine* engine, FontUtil& fontUtil, const std::string& label, int posX, int posY, bool enabled = true);
	~MenuButton() override;

	void render(Surface& targetSurface) override;

	[[nodiscard]] bool checkCollision(int x, int y) const; // TODO: abstract into RectangleCollider class?

	void mouseMoved(int x, int y);

	bool clickButton(Press press, MouseButton mouseButton, int x, int y);

	void setEnabled(bool newEnabled);
	[[nodiscard]] bool getEnabled() const;

private:
	BaseEngine* engine;
	FontUtil& fontUtil;
	const std::string label;
	int posX, posY;
	int width, height;
	Surface surface;
	bool enabled;
	bool hover, active;

	const Colour colourBackground = Colour(Colour::BLACK);
	const Colour colourNoHover = Colour(0xA0A0A0);
	const Colour colourHover = Colour(Colour::WHITE);
	const Colour colourActive = Colour(0xFFD300);
	static const enum FontSize fontSize = FontSize::MEDIUM;

	void redraw();
};

#endif
