#pragma once

#include "core/AbsHelper.hpp"
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

template <class OrbConfig>
std::optional<AspectRetType> HarmonicAspectBetween(const swe::Planet &p1,
                                                   const swe::Planet &p2) {
  double max_orb = GetMaxOrb<OrbConfig>(p1, p2);
  double diff = static_cast<double>(Min(ArcPair(p1.Lon(), p2.Lon())));
  auto p = std::lower_bound(Harmonics.begin(), Harmonics.end(),
                            diff + std::numeric_limits<double>::epsilon());

  auto ref_it = p;
  if ((diff + max_orb) > *p) {
  } else if ((diff - max_orb) < *std::next(p, -1)) {
    ref_it = std::next(p, -1);
  } else
    return std::nullopt;

  auto dist = std::distance(Harmonics.begin(), ref_it);

  double diff_2 =
      static_cast<double>(Min(ArcPair(p1.FutureLon(), p2.FutureLon())));

  auto asd = std::fabs((*ref_it) - diff);
  auto basd = std::fabs((*ref_it) - diff_2);
  return AspectRetType{
      static_cast<AspectType>(dist),
      (asd > basd) ? AspectDetail::Applying : AspectDetail::Seperating, asd};
}
template <std::equality_comparable T> bool IsInBetween(T a, T b, T val) {
  return (a < val) && (val < b) || (a > val) && (val > b);
}

template <class OrbConfig>
std::optional<AspectRetType> DeclineAspectBetween(const swe::Planet &p1,
                                                  const swe::Planet &p2) {
  double max_orb = GetMaxOrb<OrbConfig>(p1, p2);
  auto [add, sub] = AddSub(p1.Lat(), p2.Lat());
  AspectType type;
  if (IsInBetween(0 - max_orb, 0 + max_orb, add)) {
    type = AspectType::Parallel;
  } else if (IsInBetween(0 - max_orb, 0 + max_orb, sub)) {
    type = AspectType::Contraparallel;
  } else {
    return std::nullopt;
  }
  return AspectRetType{type, AspectDetail::Seperating, 10};
}
}; // namespace core
}; // namespace specni
