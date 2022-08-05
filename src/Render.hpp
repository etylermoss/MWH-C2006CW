#ifndef __H_RENDER__
#define __H_RENDER__

#include "Time.hpp"
#include "Surface.hpp"

class Render
{
public:
	Render();
	virtual ~Render() = 0;

	virtual void render(Surface& targetSurface) = 0;
	[[nodiscard]] virtual bool getShouldRender() const;

protected:
	virtual void setShouldRender(bool newShouldRender);

private:
	bool shouldRender;
};

#endif
