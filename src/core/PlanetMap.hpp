#pragma once

#include <core/swephpp.hpp>

namespace specni::core {

class PlanetMap {
public:
	PlanetMap(const swe::Ut& ut, std::vector<swe::Ipl> vIpl);

	std::unordered_map<swe::Ipl, swe::Planet> m;
	std::vector<swe::Ipl> v;
};
} // namespace specni
