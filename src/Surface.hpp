#ifndef __H_SURFACE__
#define __H_SURFACE__

#include "lib/header.h"
#include "lib/DrawingSurface.h"
#include "lib/FilterPoints.h"
#include "lib/SimpleImage.h"

#include "Colour.hpp"
#include "Filter.hpp"
#include "SurfaceBoundsFilter.hpp"

/* Here I wrap DrawingSurface so that I can apply SDL optimisations (with conditional compilation), as well as provide
 * additional features, such as optional pixel wrapping (i.e. pixel 0 - 1 == width - 1).
 * Decided not to use inheritance, but aggregation, so I can provide a cleaner interface.
*/

class Surface
{
public:
	Surface(BaseEngine* engine, int width, int height);
	Surface(BaseEngine* engine, const std::string& imagePath); /* because RawImageData::loadImage does not support alpha channels */
	~Surface();

	[[nodiscard]] DrawingSurface& getDrawingSurface();
	[[nodiscard]] SDL_Surface* getSDLSurface();

	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;
	[[nodiscard]] Colour getPixel(int x, int y) const;
	[[nodiscard]] bool isAlpha(int x, int y) const;

	void setFilter(Filter* filter);
	[[nodiscard]] Filter* getFilter();

	void setWrapping(bool newWrapping);
	[[nodiscard]] bool getWrapping() const;

	void resetChanged();
	[[nodiscard]] bool getChanged() const;

	/* drawing functions */

	void drawFill(const Colour& colour);
	void drawFillTransparent(const Colour& colour);
	void drawPixel(int x, int y, const Colour& colour);
	void drawLine(int x1, int y1, int x2, int y2, int thickness, const Colour& colour);
	void drawOval(int x, int y, int width, int height, const Colour& colour);
	void drawRectangle(int x, int y, int width, int height, const Colour& colour);
	void drawString(int x, int y, const std::string& str, Font* font, const Colour& colour);
	void drawRectangleFrom(Surface& source, int x, int y, int width, int height, int sourceX = 0, int sourceY = 0);
	void drawRectangleFromWrapping(Surface& source, int x, int y, int width, int height, int sourceX = 0, int sourceY = 0);
	void drawSimpleImage(SimpleImage& source, int x, int y, int width = 0, int height = 0, int sourceX = 0, int sourceY = 0);
	void drawFromSurface(Surface& source);

private:
	BaseEngine* engine;
	DrawingSurface drawingSurface;
	SurfaceBoundsFilter surfaceBoundsFilter;
	int surfaceWidth, surfaceHeight;
	bool changed;

	void updateChanged();
};

#endif
