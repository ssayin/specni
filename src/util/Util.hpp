#pragma once

#include <cmath>
#include <core/Aspects.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

#include <core/EssentialState.hpp>

namespace specni {
namespace util {

enum class Sign {
  Aries,
  Taurus,
  Gemini,
  Cancer,
  Leo,
  Virgo,
  Libra,
  Scorpio,
  Sagittarius,
  Capricorn,
  Aquarius,
  Pisces
};

inline const char *SignToString(Sign sign) {
  static char signs[][4]{"Ari", "Tau", "Gem", "Can", "Leo", "Vir",
                         "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"};
  return signs[static_cast<int>(sign)];
}

inline constexpr float DegToRad(float deg) { return (M_PI * deg) / 180; }

constexpr std::tuple<int, int, int> degtodms(double deg) {
  int sec = static_cast<int>(std::round(deg * 3600));
  int d = sec / 3600;
  sec = std::abs(sec % 3600);
  int min = sec / 60;
  sec %= 60;
  return std::make_tuple(d, min, sec);
}

constexpr double dmstodeg(int deg, int min, int sec) {
  return deg + min / 60.0 + sec / 3600.0;
}

const std::string get_sign_deg(/* longitude */ double lon);

const std::string AspectToString(Aspect asp);

const std::string EssentialStateToString(EssentialState state);
} // namespace util
} // namespace specni
