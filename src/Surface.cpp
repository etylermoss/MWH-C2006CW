#include <stdexcept>
#include <sstream>
#include <string>

#include "lib/header.h"
#include "lib/DrawingSurface.h"

#include "Constants.hpp"
#include "Filter.hpp"
#include "Engine.hpp"

#include "Surface.hpp"

Surface::Surface(BaseEngine* engine, int width, int height) :
	engine(engine),
	drawingSurface(engine),
	surfaceBoundsFilter(false),
	surfaceWidth(width),
	surfaceHeight(height),
	changed(true)
{
	if (width < 1 || height < 1)
		throw std::invalid_argument("Surface::Surface() width and height must not be < 1");

	drawingSurface.createSurface(width, height);
	drawingSurface.setDrawPointsFilter(&surfaceBoundsFilter);
	drawingSurface.fillSurface(0x000000);
}

Surface::Surface(BaseEngine *engine, const std::string& imagePath) :
	engine(engine),
	drawingSurface(engine),
	surfaceBoundsFilter(false),
	changed(true)
{
	/* MUST use SDL functions here because RawImageData::loadImage ignores the alpha channel. I could work around this
	 * by using a specific colour as the mask, or loading a seperate image as a mask, but in both cases it would be
	 * slower because each pixel would have to be checked when rendering it - very slow. SDL provides a way to speed
	 * this up (SDL_SRCCOLORKEY), but then using SDL functions... So might as well skip all the nonsense here and use
	 * the alpha channel. Additionally, I don't want to use the specific colour method because it is limiting and makes
	 * sprite designing harder.
	*/

	SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());

	if (!imageSurface) {
		std::stringstream errMsg;
		errMsg << "Surface::Surface(), could not load image " << imagePath << ".";
		throw std::runtime_error(errMsg.str());
	}

	surfaceWidth = imageSurface->w;
	surfaceHeight = imageSurface->h;

	drawingSurface.createSurface(surfaceWidth, surfaceHeight);

	SDL_Rect srcRect = { 0, 0, surfaceWidth, surfaceHeight };
	SDL_Rect destRect = { 0, 0, surfaceWidth, surfaceHeight };
	SDL_BlitSurface(imageSurface, &srcRect, getSDLSurface(), &destRect);

	SDL_FreeSurface(imageSurface);

	drawingSurface.setDrawPointsFilter(&surfaceBoundsFilter);
}

Surface::~Surface()
{
	SDL_FreeSurface(drawingSurface); /* WHY IS THIS NOT DONE IN DRAWING SURFACE? */
}

DrawingSurface& Surface::getDrawingSurface()
{
	return drawingSurface;
}

SDL_Surface* Surface::getSDLSurface()
{
	return drawingSurface;
}

int Surface::getWidth() const
{
	return surfaceWidth;
}

int Surface::getHeight() const
{
	return surfaceHeight;
}

Colour Surface::getPixel(int x, int y) const
{
	return Colour(drawingSurface.rawGetPixel(x, y));
}

bool Surface::isAlpha(int x, int y) const
{
	return getPixel(x, y).isAlpha();
}

void Surface::setFilter(Filter* filter)
{
	drawingSurface.setDrawPointsFilter(filter);
}

Filter* Surface::getFilter()
{
	return dynamic_cast<Filter*>(drawingSurface.getDrawPointsFilter());
}

void Surface::setWrapping(bool wrapping)
{
	surfaceBoundsFilter.setWrapping(wrapping);
}

bool Surface::getWrapping() const
{
	return surfaceBoundsFilter.getWrapping();
}

void Surface::resetChanged()
{
	changed = false;
}

bool Surface::getChanged() const
{
	return changed;
}

/* drawing functions */

/* I use this layer of indirection so that I can provide a consistent interface, and maybe specific optimisations using
 * SDL functions hidden behind conditional compilation.
 *
*/

void Surface::drawFill(const Colour& colour)
{
	drawingSurface.fillSurface(colour.getColourVal());
	updateChanged();
}

void Surface::drawFillTransparent(const Colour& colour) /* same as drawingSurface.fillSurface but respects alpha */
{
	SDL_Rect rect; rect.x = 0; rect.y = 0; rect.w = surfaceWidth; rect.h = surfaceHeight;
	SDL_FillRect(drawingSurface, &rect, colour.getColourVal()); // TODO: use STRICT_NO_SDL, just bit set every pixel
}

void Surface::drawPixel(int x, int y, const Colour& colour)
{
	drawingSurface.setPixel(x, y, colour.getColourVal());
	updateChanged();
}

void Surface::drawLine(int x1, int y1, int x2, int y2, int thickness, const Colour& colour)
{
	drawingSurface.drawThickLine(x1, y1, x2, y2, colour.getColourVal(), thickness);
	updateChanged();
}

void Surface::drawOval(int x, int y, int width, int height, const Colour& colour)
{
	if (width < 1 || height < 1)
		throw std::logic_error("Surface::drawOval, width && height must not be < 1");
	else if (width == 1 && height == 1)
		drawPixel(x, y, colour);
	else
		drawingSurface.drawOval(x, y, x + width, y + height, colour.getColourVal());

	updateChanged();
}

void Surface::drawRectangle(int x, int y, int width, int height, const Colour& colour)
{
	if (width < 1 || height < 1)
		throw std::logic_error("Surface::drawRectangle, width && height must not be < 1");

	drawingSurface.drawRectangle(x, y, x + width - 1, y + height - 1, colour.getColourVal());
	updateChanged();
}

void Surface::drawString(int x, int y, const std::string& str, Font* font, const Colour& colour)
{
	drawingSurface.drawFastString(x, y, str.c_str(), colour.getColourVal(), font);
	updateChanged();
}

void Surface::drawRectangleFrom(Surface& source, int x, int y, int width, int height, int sourceX, int sourceY)
{
#if STRICT_NO_SDL
	for (int offsetX = 0; offsetX < width; ++offsetX)
		for (int offsetY = 0; offsetY < height; ++offsetY)
			drawPixel(x + offsetX, y + offsetY, source.getPixel(sourceX + offsetX, sourceY + offsetY));
#else
	SDL_Rect srcRect = { sourceX, sourceY, sourceX + width, sourceY + height };
	SDL_Rect destRect = { x, y, width, height };
	SDL_BlitSurface(source.getSDLSurface(), &srcRect, getSDLSurface(), &destRect);
#endif
	updateChanged();
}

void Surface::drawRectangleFromWrapping(Surface& source, int x, int y, int width, int height, int sourceX, int sourceY)
{
	// TODO: check offset / source is correct
	if (x >= 0 && x + width < surfaceWidth && y >= 0 && y + height < surfaceHeight) {
		drawRectangleFrom(source, x, y, width, height, sourceX, sourceY);
	} else {
		/* nasty code to draw the wrapped rectangle in each direction */
		// Centre
		drawRectangleFrom(source, x, y, width, height, sourceX, sourceY);
		int tX, tY;
		// NW
		if (x < 0 && y < 0)
			tX = x + surfaceWidth; tY = y + surfaceHeight; drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
		// N
		if (y < 0 && x + width >= 0 && x < surfaceWidth)
			tX = x; tY = y + surfaceHeight;                drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
		// NE
		if (y < 0 && x >= surfaceWidth)
			tX = x - surfaceWidth; tY = y + surfaceHeight; drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
		// E
		if (x + width >= surfaceWidth && y < surfaceHeight && y + height >= 0)
			tX = x - surfaceWidth; tY = y;                 drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
		// SE
		if (x + width >= surfaceWidth && y + height >= surfaceHeight)
			tX = x - surfaceWidth; tY = y - surfaceHeight; drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
		// S
		if (y + height >= surfaceHeight && x + width >= 0 && x < surfaceWidth)
			tX = x; tY = y - surfaceHeight;                drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
		// SW
		if (x < 0 && y + height >= surfaceHeight)
			tX = x + surfaceWidth; tY = y - surfaceHeight; drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
		// W
		if (x < 0 && y < surfaceHeight && y + height > 0)
			tX = x + surfaceWidth; tY = y;                 drawRectangleFrom(source, tX, tY, width, height, sourceX, sourceY);
	}
	updateChanged();
}

void Surface::drawSimpleImage(SimpleImage& source, int x, int y, int width, int height, int sourceX, int sourceY)
{
	width = width == 0 ? source.getWidth() : width;
	height = height == 0 ? source.getHeight() : height;
	Engine* engineDerived = dynamic_cast<Engine*>(engine); /* temporarily disable coordinate translation since below function stupidly uses the filter on the foreground surface (WHY?) */
	engineDerived->disableScalingFilter();
	source.renderImageBlit(engine, &drawingSurface, x, y, width, height, sourceX, sourceY, width, height);
	engineDerived->enableScalingFilter();
}

/* surfaces must be the same size! */
void Surface::drawFromSurface(Surface& source)
{
	drawingSurface.copyEntireSurface(&source.drawingSurface);
	updateChanged();
}

void Surface::updateChanged()
{
	changed = true;
}
