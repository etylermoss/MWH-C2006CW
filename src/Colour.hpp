#ifndef __H_COLOUR__
#define __H_COLOUR__

#include <cstdint>

typedef uint32_t colour_t;

class Colour
{
public:

	explicit constexpr Colour() noexcept
	{
		colourVal = Colour::DEFAULT.colourVal;
	};

	explicit constexpr Colour(colour_t colourVal) noexcept :
		colourVal(colourVal)
	{
	};

	constexpr Colour(const Colour& copyOfColour) noexcept
	{
		colourVal = copyOfColour.colourVal;
	};

	constexpr ~Colour() = default;

	constexpr Colour& operator = (const Colour& rhs);
	constexpr Colour& operator = (const colour_t& rhs);
	friend constexpr bool operator == (const Colour& lhs, const Colour& rhs);
	friend constexpr bool operator != (const Colour& lhs, const Colour& rhs);
	explicit constexpr operator colour_t () const;

	void setColourVal(colour_t newColourVal);
	[[nodiscard]] colour_t getColourVal() const;

	[[nodiscard]] constexpr bool isAlpha() const
	{
		return (colourVal & 0xFF000000) > 0;
	}

	static const Colour DEFAULT;
	static const Colour BLACK;
	static const Colour WHITE;
	static const Colour RED;
	static const Colour GREEN;
	static const Colour BLUE;

private:
	uint32_t colourVal;
};

#endif
