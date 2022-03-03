#pragma once

#include <core/swephpp.hpp>

namespace specni {

namespace core {

struct PlanetMap {
  PlanetMap(const swe::Ut &ut, const std::vector<swe::Ipl> &vIpl);
  std::unordered_map<swe::Ipl, swe::Planet> m;
  std::vector<swe::Ipl> v;
};
}; // namespace core
}; // namespace specni
