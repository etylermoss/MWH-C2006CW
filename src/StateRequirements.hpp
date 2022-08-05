#ifndef __H_STATE_REQUIREMENTS__
#define __H_STATE_REQUIREMENTS__

enum class StateKey
{
	STATE_MENU,
	STATE_LEVEL,
	STATE_MAP,
	STATE_EXIT,
};

struct StateRequirements
{
public:
	explicit StateRequirements(StateKey stateKey);
	virtual ~StateRequirements() = 0;

	StateKey getStateKey();

private:
	StateKey stateKey;
};

#endif
