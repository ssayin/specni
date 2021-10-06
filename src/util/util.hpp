#pragma once

#include <cmath>
#include <include/Aspects.hpp>
#include <string>
#include <utility>

namespace specni {
namespace util {

inline float DegToRad(float deg) { return (M_PI * deg) / 180; }

const std::string get_sign_deg(/* longitude */ double lon);

inline const std::string AspectToString(Aspect asp) {
  switch (asp) {
  case Conjunction:
    return "Conjunction";
  case Sextile:
    return "Sextile";
  case Square:
    return "Square";
  case Trine:
    return "Trine";
  case Opposition:
    return "Opposition";
  default:
    return "";
  }
}
} // namespace util
} // namespace specni
