#ifndef __H_TICK__
#define __H_TICK__

#include "Time.hpp"

class Tick
{
public:
	Tick();
	virtual ~Tick() = 0;

	virtual void tick() = 0;

	void setFinished(bool newFinished);
	[[nodiscard]] bool getFinished() const;

private:
	bool finished;
};

#endif
