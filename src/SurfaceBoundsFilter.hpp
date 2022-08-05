#ifndef __H_SURFACE_BOUNDS_FILTER__
#define __H_SURFACE_BOUNDS_FILTER__

#include "lib/header.h"
#include "lib/DrawingSurface.h"

#include "Filter.hpp"
#include "Colour.hpp"

/* prevents drawing outside the surface, or wraps the pixel and option */

class SurfaceBoundsFilter : public Filter
{
public:
	explicit SurfaceBoundsFilter(bool wrapping, Filter* nextFilter = nullptr);

	bool filter(DrawingSurface* surface, int& xVirtual, int& yVirtual, colour_t& uiColour) override;
	int filterConvertVirtualToRealXPosition(int xVirtual) override;
	int filterConvertVirtualToRealYPosition(int yVirtual) override;
	int filterConvertRealToVirtualXPosition(int xReal) override;
	int filterConvertRealToVirtualYPosition(int yReal) override;

	void setWrapping(bool newWrapping);
	[[nodiscard]] bool getWrapping() const;

private:
	bool wrapping;
};

#endif
