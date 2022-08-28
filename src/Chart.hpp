#pragma once

#include "swephpp.hpp"
#include <optional>
#include <set>

namespace specni::core {

class Chart {
public:
  Chart(const swe::Ut &ut, const swe::Coordinate &geodetic, swe::HouseSystem hs,
        const std::vector<swe::Ipl> &vIpl);

  swe::House houses;
  std::set<swe::Planet> planets;
};
} // namespace specni::core
