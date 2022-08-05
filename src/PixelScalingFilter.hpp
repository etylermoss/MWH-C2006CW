#ifndef __H_PIXEL_SCALING_FILTER__
#define __H_PIXEL_SCALING_FILTER__

#include "lib/header.h"
#include "lib/DrawingSurface.h"

#include "Filter.hpp"
#include "Colour.hpp"

/* Based off of lib/ExampleFilterPointClasses.h, simplified, so I could understand it better, and for performance
 * reasons, since I only need uniform (> 1) scaling.
*/

class PixelScalingFilter : public Filter
{
public:
	explicit PixelScalingFilter(int scaleFactor);

	bool filter(DrawingSurface* surface, int& xVirtual, int& yVirtual, colour_t& uiColour) override;
	int filterConvertVirtualToRealXPosition(int xVirtual) override;
	int filterConvertVirtualToRealYPosition(int yVirtual) override;
	int filterConvertRealToVirtualXPosition(int xReal) override;
	int filterConvertRealToVirtualYPosition(int yReal) override;

private:
	int scaleFactor;
};

#endif