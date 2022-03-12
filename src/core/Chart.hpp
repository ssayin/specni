#pragma once

#include "swephpp.hpp"
#include <core/PlanetMap.hpp>
#include <optional>
namespace specni::core {

class Chart {
public:
	Chart(const swe::Ut& ut, const swe::Coordinate& geodetic, swe::HouseSystem hs,
			const std::vector<swe::Ipl>& vIpl);

	swe::House houses;
	PlanetMap planets;

private:
};
} // namespace specni
