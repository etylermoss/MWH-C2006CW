#ifndef __H_MAP_VIEW__
#define __H_MAP_VIEW__

#include <array>
#include <optional>
#include <tuple>

#include "lib/header.h"
#include "lib/BaseEngine.h"

#include "Render.hpp"
#include "Surface.hpp"
#include "SpriteManager.hpp"
#include "TimeManager.hpp"
#include "MapCountry.hpp"

//class MapView : public Render
//{
//public:
//	MapView(BaseEngine* engine, SpriteManager& spriteManager, TimeManager& timeManager);
//	~MapView() override;
//
//
//	std::optional<MapCountry*> getCountryAtWorldPos(int worldX, int worldY) {
//		for (auto& country : countries)
//			if (country.collides(worldX, worldY))
//				return &country;
//
//		return std::nullopt;
//	}
//
//};
//
//#endif
