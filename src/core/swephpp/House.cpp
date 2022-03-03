#include "House.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

namespace specni {
namespace core {
namespace swe {

House::House(const Ut &ut, ZodiacType ztype, Coordinate geodetic,
             HouseSystem hsys) {
  constexpr std::size_t GauquelinSize = 37;
  constexpr std::size_t DefaultSize = 13;

  (hsys != HouseSystem::Gauquelin) ? cusps.resize(DefaultSize)
                                   : cusps.resize(GauquelinSize);
  swe_houses(ut, geodetic.lat, geodetic.lon,
             static_cast<std::underlying_type_t<HouseSystem>>(hsys), &cusps[0],
             reinterpret_cast<double *>(&ang));
}
}; // namespace swe

}; // namespace core
}; // namespace specni
