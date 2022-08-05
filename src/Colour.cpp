#include "Colour.hpp"

constexpr Colour& Colour::operator = (const Colour& rhs)
{
	colourVal = rhs.colourVal;
	return *this;
}

constexpr Colour& Colour::operator = (const colour_t& rhs)
{
	colourVal = rhs;
	return *this;
}

constexpr bool operator == (const Colour &lhs, const Colour& rhs)
{
	return lhs.colourVal == rhs.colourVal;
}

constexpr bool operator != (const Colour &lhs, const Colour& rhs)
{
	return !(operator==(lhs, rhs));
}

constexpr Colour::operator colour_t () const
{
	return colourVal;
}

void Colour::setColourVal(colour_t newColourVal)
{
	colourVal = newColourVal;
}

colour_t Colour::getColourVal() const
{
	return colourVal;
}

const Colour Colour::DEFAULT = Colour(0xFF000000);
const Colour Colour::BLACK =   Colour(0xFF000000);
const Colour Colour::WHITE =   Colour(0xFFFFFFFF);
const Colour Colour::RED =     Colour(0xFFFF0000);
const Colour Colour::GREEN =   Colour(0xFF00FF00);
const Colour Colour::BLUE =    Colour(0xFF0000FF);