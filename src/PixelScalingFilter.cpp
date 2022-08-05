#include <stdexcept>

#include "Colour.hpp"

#include "PixelScalingFilter.hpp"

PixelScalingFilter::PixelScalingFilter(int scaleFactor) :
	Filter(false),
	scaleFactor(scaleFactor)
{
	if (scaleFactor < 1)
		throw std::invalid_argument("scaleFactor must be >= 1");
}

bool PixelScalingFilter::filter(DrawingSurface* surface, int& xVirtual, int& yVirtual, colour_t& uiColour)
{
	for (int x = scaleFactor; x > 0; x--) {
		for (int y = scaleFactor; y > 0; y--) {
			int realX = (xVirtual * scaleFactor) + x - 1;
			int realY = (yVirtual * scaleFactor) + y - 1;
			surface->rawSetPixel(realX, realY, uiColour);
		}
	}

	return false;
}

int PixelScalingFilter::filterConvertVirtualToRealXPosition(int xVirtual)
{
	return xVirtual * scaleFactor;
}

int PixelScalingFilter::filterConvertVirtualToRealYPosition(int yVirtual)
{
	return yVirtual * scaleFactor;
}

int PixelScalingFilter::filterConvertRealToVirtualXPosition(int xReal)
{
	return xReal / scaleFactor;
}

int PixelScalingFilter::filterConvertRealToVirtualYPosition(int yReal)
{
	return yReal / scaleFactor;
}