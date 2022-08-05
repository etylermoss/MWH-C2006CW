#include <sysexits.h>

#include "lib/header.h"

#include "Constants.hpp"
#include "Time.hpp"

#include "Engine.hpp"

/* Disgusting code, but needed since in BaseEngine, TTF_init() and FontManager::loadFonts() aren't called until the
 * initialization function. This means drawing of strings cannot be done in constructors, which would make the code
 * needlessly complicated. Since we can't modify BaseEngine, here I access m_oFontManager with some template tricks.
 * This class can then be constructed first by Engine. */

struct BaseEngine_fontmanager {
	typedef FontManager BaseEngine::*type;
	friend type get(BaseEngine_fontmanager);
};

template<typename Tag, typename Tag::type M>
struct Steal {
	friend typename Tag::type get(Tag) {
		return M;
	}
};

template struct Steal<BaseEngine_fontmanager, &BaseEngine::m_oFontManager>;

FontFix::FontFix(BaseEngine* engine)
{
	TTF_Init();
	FontManager* fontManager = &(engine->*get(BaseEngine_fontmanager()));
	fontManager->loadFonts();
}

/* Here I heavily modify the update / render pipeline, since I only need one surface here, and the various states
 * contain their own foreground / background objects, and are in control of their own rendering.
 *
*/

Engine::Engine() :
	BaseEngine(),
	FontFix(this),
	pixelFilter(SCALING_FACTOR),
	inputManager(*this, pixelFilter),
	spriteManager(*this),
	timeManager(*this),
	fontUtil(*this),
	stateManager(this, inputManager, spriteManager, timeManager, fontUtil),
	viewSurface(this, VIEW_WIDTH, VIEW_HEIGHT),
	timeTickPrev(0),
	timeFramePrev(0),
	timeLastInput(0)
{
	SDL_SetRenderDrawBlendMode(m_pSDL2Renderer, SDL_BLENDMODE_BLEND);
}

Engine::~Engine() = default;

int Engine::virtInitialise()
{
	fontUtil.loadAllFonts();
	return 0;
}

void Engine::virtCreateSurfaces()
{
	m_oTheBackgroundSurface.createSurface(WINDOW_WIDTH, WINDOW_HEIGHT); /* Don't actually use it, but must create it */
	m_oTheForegroundSurface.createSurface(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_oTheForegroundSurface.setDrawPointsFilter(&pixelFilter);
}

bool Engine::virtCheckForUpdateTime()
{
	return false;
}

void Engine::virtMainLoopPostUpdate()
{
	constexpr const Time minimumTickDelta {0.0, 1.0 / TICK_RATE_S};
	const Time timeTickCurr (timeManager.getTime());

	if (timeLastInput < timeTickPrev) /* Always tick after recent inputs */
		if (timeTickCurr - timeTickPrev < minimumTickDelta)
			return;

	timeTickPrev = timeTickCurr;

	stateManager.tick();

	if (stateManager.getShouldExit())
		this->setExitWithCode(EX_OK);
}

void Engine::virtRenderScreen()
{
	if (this->m_iExitWithCode >= 0) /* If due to exit, skip rendering */
		return;

	constexpr const Time minimumFrameDelta {0.0, 1.0 / MAX_FRAME_RATE_S};
	const Time timeFrameCurr (timeManager.getTime());

	if (MAX_FRAME_RATE_S != 0 && Time(timeFrameCurr - timeFramePrev) < minimumFrameDelta)
		return;

	timeFramePrev = timeFrameCurr;

	viewSurface.drawFill(Colour::BLACK);
	stateManager.render(viewSurface);

	/* Copy view surface to foreground surface, scaling in the process (uses PixelScalingFilter OR SDL functions) */
	copySurfaceToSurfaceScaled(viewSurface.getDrawingSurface(), *m_pForegroundSurface);

	/* Render surface to the display */
	m_pForegroundSurface->mySDLUpdateTexture(m_pSDL2Texture);
	SDL_RenderCopy(m_pSDL2Renderer, m_pSDL2Texture, nullptr, nullptr);
	SDL_RenderPresent(m_pSDL2Renderer);
}

void Engine::virtCleanUp()
{
}

void Engine::virtKeyDown(int iKeyCode)
{
	updateTimeLastInput();
	inputManager.keyPress(Press::DOWN, static_cast<SDL_KeyCode>(iKeyCode));
}

void Engine::virtKeyUp(int iKeyCode)
{
	updateTimeLastInput();
	inputManager.keyPress(Press::UP, static_cast<SDL_KeyCode>(iKeyCode));
}

void Engine::virtMouseDown(int iButton, int iX, int iY)
{
	updateTimeLastInput();
	updateMouseInfo(); // TODO: probably not needed?
	inputManager.mousePress(Press::DOWN, iButton, iX, iY);
}

void Engine::virtMouseUp(int iButton, int iX, int iY)
{
	updateTimeLastInput();
	updateMouseInfo();
	inputManager.mousePress(Press::UP, iButton, iX, iY);
}

void Engine::virtMouseMoved(int iX, int iY)
{
	updateTimeLastInput();
	inputManager.mouseMoved(iX, iY);
}

void Engine::virtMouseWheel(int x, int y, int which, int timestamp)
{
	updateTimeLastInput();
	inputManager.mouseWheel(x, y, which, timestamp);
}

void Engine::disableScalingFilter()
{
	m_oTheForegroundSurface.setDrawPointsFilter(nullptr);
}

void Engine::enableScalingFilter()
{
	m_oTheForegroundSurface.setDrawPointsFilter(&pixelFilter);
}

void Engine::updateTimeLastInput()
{
	timeLastInput = timeManager.getTime();
}

void copySurfaceToSurfaceScaled(DrawingSurface& fromSurface, DrawingSurface& toSurface)
{
#if STRICT_NO_SDL
	/* Must copy pixel-by-pixel so the scaling filter is applied */
	int fromW = fromSurface.getSurfaceWidth();
	int fromH = fromSurface.getSurfaceHeight();
	for (int fromX = 0; fromX < fromW; fromX++) {
		for (int fromY = 0; fromY < fromH; fromY++) {
			toSurface.setPixel(fromX, fromY, fromSurface.getPixel(fromX, fromY));
		}
	}
#else
	/* Much quicker than above, but uses SDL functions */
	SDL_Surface *fromSurfaceSDL = fromSurface.operator SDL_Surface *();
	SDL_Surface *toSurfaceSDL = toSurface.operator SDL_Surface *();
	SDL_BlitScaled(fromSurfaceSDL, nullptr, toSurfaceSDL, nullptr);
#endif
}