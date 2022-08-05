#ifndef __H_CONSTANTS__
#define __H_CONSTANTS__

#include <ratio>

#define STRICT_NO_SDL false

constexpr int TICK_RATE_S = 64;
constexpr int MAX_FRAME_RATE_S = 60;

constexpr int TILE_SIZE = 16;
constexpr int MAP_WIDTH = 32;
constexpr int MAP_HEIGHT = 18;
constexpr int VIEW_WIDTH = MAP_WIDTH * TILE_SIZE;
constexpr int VIEW_HEIGHT = MAP_HEIGHT * TILE_SIZE;
constexpr int MAP_TOTAL_WIDTH = 585;
constexpr int MAP_TOTAL_HEIGHT = 488;
constexpr int SCALING_FACTOR = 3;
constexpr int WINDOW_WIDTH = VIEW_WIDTH * SCALING_FACTOR;
constexpr int WINDOW_HEIGHT = VIEW_HEIGHT * SCALING_FACTOR;

static_assert(VIEW_WIDTH == 512 && VIEW_HEIGHT == 288, "View must be 512x288");

#endif