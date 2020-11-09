#include "planetinfo.h"

// clang-format off
#include "swe/swephexp.h"
#include "swe/sweph.h"
// clang-format on

#include <sstream>

void planet_info::set_ut(double ut) { this->ut = ut; }

double planet_info::get_ut() { return ut; }

void planet_info::set_planetid(int id) { this->id = id; }

void planet_info::calc() {
  swe_calc_ut(ut, id, SEFLG_SWIEPH, reinterpret_cast<double *>(&data), 0);
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
