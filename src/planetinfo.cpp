#include "planetinfo.h"

// clang-format off
extern "C" {
#include "swe/swephexp.h"
#include "swe/sweph.h"
}
// clang-format on

#include <cmath>
#include <sstream>
#include <tuple>

const std::tuple<std::string, double> planet_info::get_sign() {
  const char *signs[] = {
      "Aries", "Taurus",  "Gemini",      "Cancer",    "Leo",      "Virgo",
      "Libra", "Scorpio", "Sagittarius", "Capricorn", "Aquarius", "Pisces",
  };

  double pos = this->data.longitude;
  int sign = pos / 30.0;
  double deg = std::fmod(pos, 30.0);

  return std::make_tuple(std::string(signs[sign]), deg);
}

void planet_info::set_ut(double ut) { this->ut = ut; }

double planet_info::get_ut() { return ut; }

void planet_info::set_planetid(int id) { this->id = id; }

void planet_info::calc() {
  swe_calc_ut(ut, id, SEFLG_SWIEPH, (double *)&data, 0);
}

const std::string planet_info::get_planet_name() {
  switch (id) {
  case SE_SUN:
    return SE_NAME_SUN;
  case SE_MERCURY:
    return SE_NAME_MERCURY;
  case SE_MOON:
    return SE_NAME_MOON;
  case SE_VENUS:
    return SE_NAME_VENUS;
  case SE_MARS:
    return SE_NAME_MARS;
  case SE_JUPITER:
    return SE_NAME_JUPITER;
  case SE_SATURN:
    return SE_NAME_SATURN;
  case SE_URANUS:
    return SE_NAME_URANUS;
  case SE_NEPTUNE:
    return SE_NAME_NEPTUNE;
  case SE_CHIRON:
    return SE_NAME_CHIRON;
  case SE_PLUTO:
    return SE_NAME_PLUTO;
  case SE_MEAN_NODE:
    return SE_NAME_MEAN_NODE;
  case SE_TRUE_NODE:
    return SE_NAME_TRUE_NODE;
  default:
    return "UNKNOWN";
  }
}
