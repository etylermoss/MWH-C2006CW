#include "Filter.hpp"

Filter::Filter(bool chainable) :
		chainable(chainable),
		nextFilter(nullptr)
{
}

void Filter::pushFilter(Filter* newNextFilter)
{
	if (!chainable || newNextFilter && !newNextFilter->chainable)
		return;

	if (nextFilter) {
		Filter* lastFilter = nextFilter;
		while (lastFilter->nextFilter)
			lastFilter = lastFilter->nextFilter;
		lastFilter->nextFilter = newNextFilter;
	} else {
		nextFilter = newNextFilter;
	}
}

Filter* Filter::popFilter()
{
	if (!chainable || !nextFilter)
		return nullptr;

	Filter* beforeLastFilter = nextFilter;
	Filter* lastFilter = nextFilter->nextFilter;

	while (lastFilter && lastFilter->nextFilter) {
		beforeLastFilter = lastFilter;
		lastFilter = lastFilter->nextFilter;
	}

	beforeLastFilter->nextFilter = nullptr;

	return lastFilter;
}


