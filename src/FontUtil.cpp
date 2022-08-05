#include "lib/header.h"
#include "lib/BaseEngine.h"
#include "lib/FontManager.h"

#include "FontUtil.hpp"

FontUtil::FontUtil(BaseEngine& engine) :
	engine(engine)
{
}

FontUtil::~FontUtil() = default;

Font* FontUtil::getFont(enum FontSize size) const
{
	return engine.getFont("PressStart2P.ttf", sizeToFontPX(size));
}

/* immediately load all fonts now, must be called after BaseEngine::initialise() to avoid error messages */
void FontUtil::loadAllFonts() const
{
	for (const FontSize fontSize : {FontSize::DEFAULT, FontSize::SMALL, FontSize::MEDIUM, FontSize::LARGE, FontSize::VERY_LARGE})
		getFont(fontSize);
}