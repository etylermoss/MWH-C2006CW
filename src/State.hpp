#ifndef __H_STATE__
#define __H_STATE__

#include <memory>

#include "Tick.hpp"
#include "Render.hpp"
#include "StateRequirements.hpp"
#include "Surface.hpp"

class State : public Render, public Tick
{
public:
	explicit State(StateKey stateKey);
	~State() override = 0;

	virtual void initialize(std::unique_ptr<StateRequirements> baseRequirements) = 0;
	virtual void deinitialize() = 0; /* called *after* getNextStateRequirements(), and on exit */
	virtual std::unique_ptr<StateRequirements> getNextStateRequirements() = 0;
	void tick() override = 0;
	void render(Surface& viewSurface) override = 0;

	[[nodiscard]] StateKey getStateKey() const;

	void setShouldExit(bool newShouldExit);
	[[nodiscard]] bool getShouldExit() const;
	
private:
	const StateKey stateKey;
	bool shouldExit;
};

#endif
