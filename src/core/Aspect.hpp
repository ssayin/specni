#pragma once

#include "core/AbsHelper.hpp"
#include <core/swephpp.hpp>
#include <functional>
#include <optional>

namespace specni {
namespace core {

namespace detail {

enum class HarmonicType : uint8_t {
  Conjunction,
  Sextile,
  Square,
  Trine,
  Opposition,
  HarmonicCount,
};

enum class DeclinationType : uint8_t {
  Parallel,
  Contraparallel,
  DeclinationCount
};

constexpr uint8_t DeclinationOffset = 0x10;

}; // namespace detail

enum class AspectType : uint8_t {
  HarmonicType = 0x0,
  DeclinationType = detail::DeclinationOffset,
};

enum class AspectDetail : uint8_t { Applying, Seperating, Count };

constexpr std::array<uint16_t, // max = full circle
                     static_cast<std::size_t>(
                         detail::HarmonicType::HarmonicCount)>
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
  auto max_orb = GetMaxOrb<OrbConfig>(p1, p2);
  auto &a = p1.Lon();
  auto &b = p2.Lon();

  for (int i = 0; i < Harmonics.size(); ++i) {
    if (a.IsWithinOrbOf(static_cast<double>(b) + Harmonics[i], max_orb) ||
        a.IsWithinOrbOf(static_cast<double>(b) - Harmonics[i], max_orb)) {
      auto asp_orb = static_cast<EclipticLongitude>(Harmonics[i]);
      auto [now, next] =
          std::pair{Min(AddSub(p1.Lon(), p2.Lon(), asp_orb)),
                    Min(AddSub(p1.FutureLon(), p2.FutureLon(), asp_orb))};

      return AspectRetType{static_cast<AspectType>(i),
                           now > next ? AspectDetail::Applying
                                      : AspectDetail::Seperating,
                           static_cast<double>(now)};
    }
  }

  return std::nullopt;
}

template <class OrbConfig>
std::optional<AspectRetType> DeclineAspectBetween(const swe::Planet &p1,
                                                  const swe::Planet &p2) {
  /*  auto [a, b] = AddSubFabs(p1.Lat(), p2.Lat());
    if (std::min(a, b) < MaxOrb<Config>(p1, p2)) {
      return std::make_tuple(A::Contraparallel, a,
                             a <= std::fabs(p1.SpdLat() + p2.SpdLat())
                                 ? A::Applying
                                 : A::Seperating);
    }
    if (std::max(a, b) < MaxOrb<Config>(p1, p2)) {
      return std::make_tuple(A::Contraparallel, b,
                             b <= std::fabs(p1.SpdLat() + p2.SpdLat())
                                 ? A::Applying
                                 : A::Seperating);
    }
  */
  return std::nullopt;
}

}; // namespace core

}; // namespace specni
