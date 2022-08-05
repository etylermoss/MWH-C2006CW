#include "StateRequirements.hpp"

StateRequirements::StateRequirements(StateKey stateKey) :
	stateKey(stateKey)
{
}

StateRequirements::~StateRequirements() = default;

StateKey StateRequirements::getStateKey()
{
	return stateKey;
}
