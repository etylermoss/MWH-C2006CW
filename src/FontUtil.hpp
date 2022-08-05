#ifndef __H_FONT_UTIL__
#define __H_FONT_UTIL__

#include "lib/BaseEngine.h"

/* I use this instead of lib/FontManager so that I can use enums for standard sizes & font types. I also use it instead
 * of methods on Engine to avoid circular dependencies (since other classes just use BaseEngine*).
*/

// TODO size to int

enum class FontSize {
	DEFAULT,
	SMALL,
	MEDIUM,
	LARGE,
	VERY_LARGE,
};

class FontUtil
{
public:
	explicit FontUtil(BaseEngine& engine);
	~FontUtil();

	Font* getFont(enum FontSize size) const;
	void loadAllFonts() const;

	[[nodiscard]] static constexpr int sizeToFontPX(enum FontSize size)
	{
		switch (size) {
		case FontSize::VERY_LARGE: return 64;
		case FontSize::LARGE:      return 32;
		case FontSize::MEDIUM:     return 16;
		case FontSize::SMALL:
		case FontSize::DEFAULT:
		default:               return 8;
		}
	}

private:
	BaseEngine& engine;
};


#endif
