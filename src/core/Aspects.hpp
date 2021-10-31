#pragma once

#include "Cyclic.hpp"
#include "Planet.hpp"
#include "imgui.h"
#include "swephpp.hpp"
#include <algorithm>
#include <array>
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

const std::array<ImVec4, None> aspectColor{
    ImVec4(1, 1, 1, 1), ImVec4(0, 1, 0, 1), ImVec4(1, 0, 0, 1),
    ImVec4(0, 1, 1, 1), ImVec4(1, 0, 0, 1),
};

struct OrbConfig {
  static constexpr int Get(swephpp::Ipl body) {
    switch (body) {
    case swephpp::Ipl::Sun:
      return 7;
    case swephpp::Ipl::Moon:
      return 6;
    case swephpp::Ipl::Mercury:
    case swephpp::Ipl::Venus:
      return 3;
    case swephpp::Ipl::Mars:
      return 4;
    case swephpp::Ipl::Jupiter:
    case swephpp::Ipl::Saturn:
      return 5;
    default:
      return 2;
    }
  }
};

struct OrbPartileConfig {
  static constexpr int Get(swephpp::Ipl body) { return 1; }
};

typedef std::vector<std::tuple<swephpp::Ipl, swephpp::Ipl, Aspect>> AspectTuple;

AspectTuple
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