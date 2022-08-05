#ifndef __H_TEXT_SCROLL__
#define __H_TEXT_SCROLL__

#include <string>
#include <vector>
#include <iostream>
#include <concepts>
#include <type_traits>

#include "lib/header.h"
#include "lib/FontManager.h"

#include "Surface.hpp"
#include "TimeManager.hpp"
#include "Tick.hpp"
#include "Render.hpp"
#include "Time.hpp"

/* Note: make TextScroll(), texts, etc. constexpr upon GCC 12 release, w/ constexpr std::vector support */

template <class T>
concept textTypeRequirements = std::is_same_v<T, std::string> or std::is_same_v<T, const char*> or std::is_same_v<T, char*> or std::is_same_v<T, char>;

class TextScroll : public Tick, public Render
{
public:
	explicit TextScroll(TimeManager& timeManager);
	TextScroll(TimeManager& timeManager, const Time& timeStep, int posX, int posY, const Colour& colour, Font* font);
	~TextScroll() override;

	template <typename T, textTypeRequirements... U>
	requires textTypeRequirements<T>
	constexpr void start(T text, U... args) {
		start(text);
		start(args...);
	}

	template <typename T>
	requires textTypeRequirements<T>
	constexpr void start(T text) {
		timePrev = timeManager.getTime();
		textsProgress = 1;
		if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, const char*> || std::is_same_v<T, char*>)
			texts.emplace_back(text);
		else if constexpr (std::is_same_v<T, char>)
			texts.emplace_back(std::string(1, text));
	}

	void tick() override;
	void render(Surface& viewSurface) override;

	[[nodiscard]] std::string createCurrentText() const;

private:
	TimeManager& timeManager;
	std::vector<std::string> texts;
	int textsProgress;
	Time timePrev, timeStep;
	int posX, posY;
	const Colour colour;
	Font* font;

};

#endif
