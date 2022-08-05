#include "lib/header.h"
#include "lib/FilterPoints.h"

#include "Colour.hpp"

#ifndef __H_FILTER__
#define __H_FILTER__

class Filter : public FilterPoints
{
public:
	explicit Filter(bool chainable);

	bool filter(DrawingSurface* surface, int& x, int& y, colour_t& uiColour) override = 0;

	/* push a filter to the end of the filter list */
	void pushFilter(Filter* newNextFilter);

	/* pop the last filter from the filter list */
	Filter* popFilter();

protected:
	const bool chainable;
	Filter* nextFilter;
};

#endif
