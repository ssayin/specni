#pragma once

#include <vector>

namespace specni {
class Planet;
using PlanetPairs = std::vector<std::pair<Planet &, Planet &>>;

// auto GetPlanetCombPairs(std::vector<Planet> &V) -> PlanetPairs;
}; // namespace specni