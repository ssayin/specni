#pragma once

#include "Cyclic.hpp"
#include "Planet.hpp"
#include "swephpp.hpp"
#include <algorithm>
#include <cassert>
#include <core/PlanetPairs.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace specni {
/*
 * Only major aspects for now
 */

enum Aspect {
  Conjunction,
  Sextile,
  Square,
  Trine,
  Opposition,
  None,
};

const std::unordered_map<Aspect, int> Aspects = {
    {Conjunction, 0}, {Sextile, 60},     {Square, 90},
    {Trine, 120},     {Opposition, 180},
};

struct OrbConfig {
  static constexpr int Get(swephpp::PlanetaryBody body) {
    switch (body) {
    case swephpp::PlanetaryBody::Sun:
      return 7;
    case swephpp::PlanetaryBody::Moon:
      return 6;
    case swephpp::PlanetaryBody::Mercury:
    case swephpp::PlanetaryBody::Venus:
      return 3;
    case swephpp::PlanetaryBody::Mars:
      return 4;
    case swephpp::PlanetaryBody::Jupiter:
    case swephpp::PlanetaryBody::Saturn:
      return 5;
    default:
      return 2;
    }
  }
};

typedef std::vector<
    std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody, Aspect>>
    AspectMatrix;

AspectMatrix
CalculateAspects(PlanetPairs &pairs,
                 std::function<Aspect(const Planet &, const Planet &)> f);

template <class Config = OrbConfig>
auto AspectFunc = [](const Planet &p1, const Planet &p2) -> Aspect {
  Longitude a(p1.Data.lon);
  Longitude b(p2.Data.lon);
  for (auto &asp : Aspects) {
    int maxOrb = std::max(Config::Get(p1.Id), Config::Get(p2.Id));
    if (a.within(b + asp.second, maxOrb) || a.within(b - asp.second, maxOrb)) {
      return asp.first;
    }
  }
  return Aspect::None;
};
}; // namespace specni