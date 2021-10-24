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

inline const std::string get_sign_deg(/* longitude */ double lon) {
  std::stringstream ss;
  int deg;
  int min;
  int sec;
  std::tie(deg, min, sec) = degtodms(std::fmod(lon, 30.0));
  ss.fill('0');
  ss << deg << "° " << std::setw(2) << SignToString(static_cast<Sign>(lon / 30))
     << ' ' << std::setw(2) << min << '\'' << std::setw(2) << sec << "\"";

  return ss.str();
}

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

inline const std::string EssentialStateToString(EssentialState state) {
  switch (state) {
  case EssentialState::Domicile:
    return "Domicile";
  case EssentialState::MutualDomicile:
    return "Mutually Domicile";
  case EssentialState::InDetriment:
    return "In Detriment";
  case EssentialState::Exalted:
    return "Exalted";
  case EssentialState::MutualExalted:
    return "Mutually Exalted";
  case EssentialState::Fallen:
    return "Fallen";
  case EssentialState::InOwnTriplicity:
    return "In Own Triplicity";
  case EssentialState::InOwnFace:
    return "In Own Face";
  case EssentialState::InOwnTerm:
    return "In Own Term";
  case EssentialState::Peregrine:
    return "Peregrine";
  default:
    return "";
  }
}
} // namespace util
} // namespace specni
