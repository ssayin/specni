#pragma once

#include "swephpp.hpp"
#include <core/PlanetMap.hpp>

namespace specni {
namespace core {

class Chart {
public:
  Chart(const swe::Ut &ut, const swe::Coordinate &geodetic, swe::HouseSystem hs,
        const std::vector<swe::Ipl> &vIpl);

private:
  swe::House houses;
  PlanetMap planets;
};
}; // namespace core

}; // namespace specni
