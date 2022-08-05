#include "Tick.hpp"

Tick::Tick() :
	finished(false)
{
}

Tick::~Tick() = default;

void Tick::setFinished(bool newFinished)
{
	finished = newFinished;
}

bool Tick::getFinished() const
{
	return finished;
}
