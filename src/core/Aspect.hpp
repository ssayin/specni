#pragma once

#include <core/Util.hpp>
#include <core/swephpp.hpp>
#include <functional>
#include <limits>
#include <optional>

namespace specni {
namespace core {

namespace detail {
constexpr uint8_t DeclinationOffset = 0xF;
}; // namespace detail

enum class AspectType : uint8_t {
  Conjunction,
  Sextile,
  Square,
  Trine,
  Opposition,
  HarmonicCount,
  Parallel = detail::DeclinationOffset,
  Contraparallel,
  DeclinationCount

};

enum class AspectDetail : uint8_t { Applying, Seperating, Count };

constexpr std::array<uint8_t, // max = full circle / 2
                     static_cast<std::size_t>(AspectType::HarmonicCount)>
    Harmonics{0, 60, 90, 120, 180};

struct AspectRetType {
  AspectType type;
  AspectDetail detail;
  double orb;
};

struct DefaultOrbConfig {
  static constexpr auto Get(swe::Ipl body) -> int {
    switch (body) {
    case swe::Ipl::Sun:
      return 7;
    case swe::Ipl::Moon:
      return 6;
    case swe::Ipl::Mercury:
    case swe::Ipl::Venus:
      return 3;
    case swe::Ipl::Mars:
      return 4;
    case swe::Ipl::Jupiter:
    case swe::Ipl::Saturn:
      return 5;
    default:
      return 2;
    }
  }
};

struct OrbPartileConfig {
  static constexpr auto Get(swe::Ipl body) -> int { return 1; }
};

template <class OrbConfig>
auto GetMaxOrb(const swe::Planet &p1, const swe::Planet &p2) {
  return std::max(OrbConfig::Get(p1.Id()), OrbConfig::Get(p2.Id()));
}

inline std::optional<decltype(Harmonics.begin())>
GetClosestBoundary(double maxAllowedOrb, double shortestArc) {
  auto it =
      std::lower_bound(Harmonics.begin(), Harmonics.end(),
                       shortestArc + std::numeric_limits<double>::epsilon());
  if ((shortestArc + maxAllowedOrb) > *it) {
    return it;
  } else if ((shortestArc - maxAllowedOrb) < *std::next(it, -1)) {
    return std::next(it, -1);
  } else {
    return std::nullopt;
  }
}

template <class OrbConfig>
std::optional<AspectRetType> HarmonicAspectBetween(const swe::Planet &p1,
                                                   const swe::Planet &p2) {
  double maxAllowed = GetMaxOrb<OrbConfig>(p1, p2);
  double actualArc = static_cast<double>(Min(ArcPair(p1.Lon(), p2.Lon())));
  auto optIt = GetClosestBoundary(maxAllowed, actualArc);

  if (!optIt.has_value())
    return std::nullopt;

  double futureArc =
      static_cast<double>(Min(ArcPair(p1.FutureLon(), p2.FutureLon())));

  auto distToExact = std::fabs((*optIt.value()) - actualArc);
  auto futureDistToExact = std::fabs((*optIt.value()) - futureArc);

  auto index = std::distance(Harmonics.begin(), optIt.value());
  return AspectRetType{static_cast<AspectType>(index),
                       (distToExact > futureDistToExact)
                           ? AspectDetail::Applying
                           : AspectDetail::Seperating,
                       distToExact};
}

template <class OrbConfig>
std::optional<AspectRetType> DeclineAspectBetween(const swe::Planet &p1,
                                                  const swe::Planet &p2) {
  double maxAllowed = GetMaxOrb<OrbConfig>(p1, p2);
  auto [sum, sub] = AddSub(p1.Lat(), p2.Lat());
  AspectType type;
  if (IsInBetween(0 - maxAllowed, 0 + maxAllowed, sum)) {
    type = AspectType::Parallel;
  } else if (IsInBetween(0 - maxAllowed, 0 + maxAllowed, sub)) {
    type = AspectType::Contraparallel;
  } else {
    return std::nullopt;
  }
  return AspectRetType{type, AspectDetail::Seperating, 10};
}
}; // namespace core
}; // namespace specni
