#pragma once

#include <vector>

namespace specni {
class Planet;
typedef std::vector<std::pair<Planet, Planet>> PlanetPairs;

PlanetPairs GetPlanetCombPairs(std::vector<Planet> &V);
}; // namespace specni