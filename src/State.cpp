#include "Tick.hpp"
#include "Render.hpp"

#include "State.hpp"

State::State(StateKey stateKey) :
	Tick(),
	Render(),
	stateKey(stateKey),
	shouldExit(false)
{
}

State::~State() = default;

[[nodiscard]] StateKey State::getStateKey() const
{
	return stateKey;
}

bool State::getShouldExit() const
{
	return shouldExit;
}

void State::setShouldExit(bool newShouldExit)
{
	shouldExit = newShouldExit;
}