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
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace specni {
/*
 * Only major aspects for now
 */

enum class AspectAngle {
  Conjunction,
  Sextile,
  Square,
  Trine,
  Opposition,
  Count,
};

enum class Declination { Parallel, Contraparallel, Count };

enum AspectStat { Applying = 0, Seperating, Count };

const std::unordered_map<AspectAngle, int> Aspects = {
    {AspectAngle::Conjunction, 0},  {AspectAngle::Sextile, 60},
    {AspectAngle::Square, 90},      {AspectAngle::Trine, 120},
    {AspectAngle::Opposition, 180},
};

struct DefaultOrbConfig {
  static constexpr auto Get(swephpp::Ipl body) -> int {
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
  static constexpr auto Get(swephpp::Ipl body) -> int { return 1; }
};

template <class A>
using AspectTuple =
    std::tuple<swephpp::Ipl, swephpp::Ipl, A, double, AspectStat>;

template <class A> using AspectTupleVector = std::vector<AspectTuple<A>>;

template <class A>
constexpr bool IsAspectAngleType = std::is_same<A, AspectAngle>{}();

template <class A>
constexpr bool IsDeclinationType = std::is_same<A, Declination>{}();

template <class A> using AspectFuncRetType = std::tuple<A, double, AspectStat>;

template <class A, class AspectConfig = DefaultOrbConfig>

// this is no longer needed
using AspFuncSignature = std::enable_if_t<
    IsAspectAngleType<A> or IsDeclinationType<A>,
    std::function<AspectFuncRetType<A>(const Planet &, const Planet &)>>;

template <class A, class AspectConfig = DefaultOrbConfig>
auto CalculateAspects(
    PlanetPairs &pairs) //, AspFuncSignature<A, AspectConfig> f)
    -> AspectTupleVector<A> {

  AspectTuple<A> ret;

  for (std::pair<Planet, Planet> p : pairs) {
    auto asp = AspectFunc<A, AspectConfig>(p.first, p.second);
    if (std::get<0>(asp) != A::Count) {
      ret.push_back({p.first.Id, p.second.Id, std::get<0>(asp),
                     std::get<1>(asp), std::get<2>(asp)});
    }
  }
  return ret;
}

static auto willNameItLater(const Longitude &first, const Longitude &second,
                            const Longitude &orb) -> const Longitude && {
  return std::move(
      std::min(std::min(second - first + orb, first - second + orb),
               std::min(second - first - orb, first - second - orb)));
}

// will generate so many functions at compile time, what to do?
template <class A, class AspectConfig>
auto AspectFunc(const Planet &p1, const Planet &p2)
    -> std::enable_if_t<IsAspectAngleType<A>, AspectFuncRetType<A>> {
  // This should give ~0 if planets are contra-parallel
  double a = std::fabs(p1.Data.lat + p2.Data.lat);
  double b = std::fabs(p1.Data.lat - p2.Data.lat);
  if (a < AspectConfig::Get(p1.Id)) {
    return std::make_tuple(Declination::Contraparallel, a,
                           a <= std::fabs(p1.Data.spdlat + p2.Data.spdlat)
                               ? Applying
                               : Seperating);
  } else if (b < AspectConfig::Get(p1.Id)) {
    return std::make_tuple(Declination::Parallel, b,
                           b <= std::fabs(p1.Data.spdlat + p2.Data.spdlat)
                               ? Applying
                               : Seperating);
  }

  return std::make_tuple(Declination::Count, 0, AspectStat::Count);
}

template <class A, class AspectConfig>
auto AspectFunc(const Planet &p1, const Planet &p2)
    -> std::enable_if_t<IsDeclinationType<A>, AspectFuncRetType<A>> {
  Longitude a(p1.Data.lon);
  Longitude b(p2.Data.lon);
  for (auto &asp : Aspects) {
    int maxOrb = std::max(AspectConfig::Get(p1.Id), AspectConfig::Get(p2.Id));
    if (a.within(b + asp.second, maxOrb) || a.within(b - asp.second, maxOrb)) {
      Longitude actualArcDistance =
          willNameItLater(a, b, asp.second); // implicit

      Longitude predictedArcDistance =
          willNameItLater(a + Longitude(p1.Data.spdlon),
                          b + Longitude(p2.Data.spdlon), asp.second);

      return std::make_tuple(asp.first, actualArcDistance(),
                             ((actualArcDistance() > predictedArcDistance())
                                  ? Applying
                                  : Seperating));
    }
  }
  return std::make_tuple(AspectAngle::Count, 0, AspectStat::Count);
};
}; // namespace specni