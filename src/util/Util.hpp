#pragma once

#include <cmath>
#include <core/Aspects.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

#include <core/EssentialState.hpp>

namespace specni::util {

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

inline auto SignToString(Sign sign) -> const char * {
  static char signs[][4]{"Ari", "Tau", "Gem", "Can", "Leo", "Vir",
                         "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"};
  return signs[static_cast<int>(sign)];
}

inline constexpr auto DegToRad(float deg) -> float {
  return (M_PI * deg) / 180;
}

constexpr auto degtodms(double deg) -> std::tuple<int, int, int> {
  int sec = static_cast<int>(std::round(deg * 3600));
  int d = sec / 3600;
  sec = std::abs(sec % 3600);
  int min = sec / 60;
  sec %= 60;
  return std::make_tuple(d, min, sec);
}

constexpr auto dmstodeg(int deg, int min, int sec) -> double {
  return deg + min / 60.0 + sec / 3600.0;
}

auto get_sign_deg(/* longitude */ double lon) -> const std::string;

auto AspectToString(AspectAngle asp) -> const std::string;

auto EssentialStateToString(EssentialState state) -> const std::string;

template <typename tuple_t>
constexpr auto get_array_from_tuple(tuple_t &&tuple) {
  constexpr auto get_array = [](auto &&...x) {
    return std::array{std::forward<decltype(x)>(x)...};
  };
  return std::apply(get_array, std::forward<tuple_t>(tuple));
}

} // namespace specni::util
