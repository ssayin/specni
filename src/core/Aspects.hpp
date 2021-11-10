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

enum class Aspect {
  Conjunction,
  Sextile,
  Square,
  Trine,
  Opposition,
  Count,
};

enum class Aspect2 { Parallel, Contraparallel, Count };

const std::unordered_map<Aspect, int> Aspects = {
    {Aspect::Conjunction, 0}, {Aspect::Sextile, 60},     {Aspect::Square, 90},
    {Aspect::Trine, 120},     {Aspect::Opposition, 180},
};

const std::array<ImVec4, static_cast<size_t>(Aspect::Count)> aspectColor{
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

enum AspectStat { Applying = 0, Seperating, No };

template <class Aspect>
using AspectTuple = std::vector<
    std::tuple<swephpp::Ipl, swephpp::Ipl, Aspect, double, AspectStat>>;

template <class Aspect>
AspectTuple<Aspect> CalculateAspects(
    PlanetPairs &pairs,
    std::function<std::tuple<Aspect, double, AspectStat>(const Planet &,
                                                         const Planet &)>
        f) {

  AspectTuple<Aspect> ret;

  for (std::pair<Planet, Planet> &p : pairs) {
    auto asp = f(p.first, p.second);
    if (std::get<0>(asp) != Aspect::Count) {
      ret.push_back({p.first.Id, p.second.Id, std::get<0>(asp),
                     std::get<1>(asp), std::get<2>(asp)});
    }
  }

  return ret;
}

static const Longitude &willNameItLater(const Longitude &first,
                                        const Longitude &second,
                                        const Longitude &orb) {
  Longitude asd = std::min(second - first + orb, first - second + orb);
  Longitude sdf = std::min(second - first - orb, first - second - orb);
  return std::min(asd, sdf);
}

// Orbs are not implemented yet
template <class Config = OrbPartileConfig>
auto aspectFunc2 =
    [](const Planet &p1,
       const Planet &p2) -> std::tuple<Aspect2, double, AspectStat> {
  // This should give ~0 if planets are contra-parallel
  double a = std::fabs(p1.Data.lat + p2.Data.lat);
  double b = std::fabs(p1.Data.lat - p2.Data.lat);
  if (a < Config::Get(p1.Id)) {
    return std::make_tuple(Aspect2::Contraparallel, a,
                           a <= std::fabs(p1.Data.spdlat + p2.Data.spdlat)
                               ? Applying
                               : Seperating);
  } else if (b < Config::Get(p1.Id)) {
    return std::make_tuple(Aspect2::Parallel, b,
                           b <= std::fabs(p1.Data.spdlat + p2.Data.spdlat)
                               ? Applying
                               : Seperating);
  }

  return std::make_tuple(Aspect2::Count, 0, AspectStat::No);
};

template <class Config = OrbConfig>
auto AspectFunc =
    [](const Planet &p1,
       const Planet &p2) -> std::tuple<Aspect, double, AspectStat> {
  Longitude a(p1.Data.lon);
  Longitude b(p2.Data.lon);
  for (auto &asp : Aspects) {
    int maxOrb = std::max(Config::Get(p1.Id), Config::Get(p2.Id));
    if (a.within(b + asp.second, maxOrb) || a.within(b - asp.second, maxOrb)) {
      Longitude degP1Next = a + Longitude(p1.Data.spdlon);

      Longitude degP2Next = b + Longitude(p2.Data.spdlon);

      AspectStat stat = No;

      Longitude actualArcDistance =
          willNameItLater(a, b, asp.second); // implicit

      Longitude predictedArcDistance =
          willNameItLater(degP1Next, degP2Next, asp.second);

      stat = ((actualArcDistance() > predictedArcDistance()) ? Applying
                                                             : Seperating);

      return std::make_tuple(asp.first, actualArcDistance(), stat);
    }
  }
  return std::make_tuple(Aspect::Count, 0, AspectStat::No);
};
}; // namespace specni