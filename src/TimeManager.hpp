#ifndef __H_TIME_MANAGER__
#define __H_TIME_MANAGER__

#include "lib/header.h"
#include "lib/BaseEngine.h"

#include "Time.hpp"

class TimeManager
{
public:
	explicit TimeManager(BaseEngine& engine);
	~TimeManager();

	[[nodiscard]] Time getTime() const;

private:
	BaseEngine& engine;
};

#endif
