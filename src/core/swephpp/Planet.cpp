#include "Planet.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

#include <iostream>
#include <tuple>

namespace specni {
namespace core {
namespace swe {

std::ostream &operator<<(std::ostream &out, Planet const &p) {
  out << p.Name() << " " << p.x.lon << " " << p.x.lat;
  return out;
}

Planet::Planet(const Ipl &ipl, const Ut &ut, const EphFlag &flag) : ipl(ipl) {
  char err[256];
  swe_calc_ut(ut, static_cast<std::underlying_type_t<Ipl>>(ipl),
              static_cast<std::underlying_type_t<EphFlag>>(flag),
              reinterpret_cast<double *>(&x), err);
}

auto Planet::Name() const -> const std::string {
  char ret[SE_MAX_STNAME];
  swe_get_planet_name(static_cast<std::underlying_type_t<Ipl>>(ipl), ret);
  return std::string(ret);
}

}; // namespace swe
}; // namespace core
}; // namespace specni
