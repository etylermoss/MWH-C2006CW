#include "Colour.hpp"

#include "SurfaceBoundsFilter.hpp"

SurfaceBoundsFilter::SurfaceBoundsFilter(bool wrapping, Filter* nextFilter) :
	Filter(true),
	wrapping(wrapping)
{
}

bool SurfaceBoundsFilter::filter(DrawingSurface* surface, int& xVirtual, int& yVirtual, colour_t& uiColour)
{
	if (nextFilter)
		nextFilter->filter(surface, xVirtual, yVirtual, uiColour);

	if (wrapping) {
		xVirtual = xVirtual % surface->getSurfaceWidth();
		yVirtual = yVirtual % surface->getSurfaceHeight();
		return true;
	}

	if (xVirtual < 0 || xVirtual >= surface->getSurfaceWidth() || yVirtual < 0 || yVirtual >= surface->getSurfaceHeight())
		return false;

	return true;
}

int SurfaceBoundsFilter::filterConvertVirtualToRealXPosition(int xVirtual)
{
	return nextFilter ? nextFilter->filterConvertVirtualToRealXPosition(xVirtual) : xVirtual;
}

int SurfaceBoundsFilter::filterConvertVirtualToRealYPosition(int yVirtual)
{
	return nextFilter ? nextFilter->filterConvertVirtualToRealYPosition(yVirtual) : yVirtual;
}

int SurfaceBoundsFilter::filterConvertRealToVirtualXPosition(int xReal)
{
	if (nextFilter)
		xReal = nextFilter->filterConvertRealToVirtualXPosition(xReal);
	return xReal;
}

int SurfaceBoundsFilter::filterConvertRealToVirtualYPosition(int yReal)
{
	if (nextFilter)
		yReal = nextFilter->filterConvertRealToVirtualYPosition(yReal);
	return yReal;
}

void SurfaceBoundsFilter::setWrapping(bool newWrapping)
{
	wrapping = newWrapping;
}

bool SurfaceBoundsFilter::getWrapping() const
{
	return wrapping;
}