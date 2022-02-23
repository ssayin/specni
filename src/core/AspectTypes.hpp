#pragma once

#include <core/swephpp.hpp>
#include <unordered_map>

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

}; // namespace specni