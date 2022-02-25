#include "Planet.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

#include <tuple>

namespace specni {
namespace core {
namespace swe {

Planet::Planet(const Ipl &ipl, const Ut &ut, const EphFlag &flag) {
  swe_calc_ut(ut, static_cast<std::underlying_type_t<Ipl>>(ipl),
              static_cast<std::underlying_type_t<EphFlag>>(flag),
              reinterpret_cast<double *>(&x), nullptr);
}

}; // namespace swe
}; // namespace core
}; // namespace specni
