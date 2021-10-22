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

inline const std::string get_sign_deg(/* longitude */ double lon) {
  std::stringstream ss;
  int sec = static_cast<int>(std::round(std::fmod(lon, 30.0) * 3600));
  int deg = sec / 3600;
  sec = std::abs(sec % 3600);
  int min = sec / 60;
  sec %= 60;

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
