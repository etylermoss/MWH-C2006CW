

#include "LevelPlayer.hpp"

float LevelPlayer::bring_towards_zero(float x, float step)
{
	bool negative = isless(x, 0.0f);

	if (negative) {
		x += step;
		x = !isless(x, 0.0) ? 0.0f : x;
	} else {
		x -= step;
		x = isless(x, 0.0) ? 0.0f : x;
	}

	return x;
}
