#ifndef __H_ENGINE__
#define __H_ENGINE__

#include "lib/header.h"
#include "lib/BaseEngine.h"
#include "lib/ImageManager.h"
#include "lib/FontManager.h"

#include "InputManager.hpp"
#include "SpriteManager.hpp"
#include "TimeManager.hpp"
#include "FontUtil.hpp"
#include "Constants.hpp"
#include "PixelScalingFilter.hpp"
#include "Time.hpp"
#include "StateManager.hpp"
#include "Surface.hpp"

class FontFix
{
public:
	explicit FontFix(BaseEngine* engine);
};

class Engine final : public BaseEngine, private FontFix
{
public:
	Engine();
	~Engine() override;

	int virtInitialise() override;
	void virtCreateSurfaces() override; /* Apply pixel scaling filter */
	bool virtCheckForUpdateTime() override; /* We clamp the tick rate elsewhere (virtMainLoopPostUpdate) */
	void virtMainLoopPostUpdate() override;
	void virtRenderScreen() override;
	void virtCleanUp() override;

	void virtKeyDown(int iKeyCode) override;
	void virtKeyUp(int iKeyCode) override;
	void virtMouseDown(int iButton, int iX, int iY) override;
	void virtMouseUp(int iButton, int iX, int iY) override;
	void virtMouseMoved(int iX, int iY) override;
	void virtMouseWheel(int x, int y, int which, int timestamp) override;

	void disableScalingFilter(); /* needed for SimpleImage::renderImageBlit, which stupidly uses coordinate translation from the engine */
	void enableScalingFilter();

private:
	InputManager inputManager;
	SpriteManager spriteManager;
	TimeManager timeManager;
	FontUtil fontUtil;
	StateManager stateManager;
	PixelScalingFilter pixelFilter;
	Surface viewSurface;
	Time timeTickPrev, timeFramePrev, timeLastInput;

	void updateTimeLastInput();
};

void copySurfaceToSurfaceScaled(DrawingSurface& fromSurface, DrawingSurface& toSurface);

#endif