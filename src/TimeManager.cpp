#include "lib/header.h"

#include "Constants.hpp"

#include "TimeManager.hpp"

TimeManager::TimeManager(BaseEngine& engine) :
	engine(engine)
{
}

TimeManager::~TimeManager()
{
}

Time TimeManager::getTime() const
{
#if STRICT_NO_SDL
	return Time(engine.getRawTime());
#else
	return Time(SDL_GetTicks(), 0l);
#endif
}