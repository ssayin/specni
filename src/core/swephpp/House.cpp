#include "House.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

namespace specni {
namespace core {
namespace swe {

House::House(const Ut &ut, ZodiacType ztype, const Coordinate &geodetic,
             HouseSystem hsys) {
  constexpr size_t GauquelinSize = 37;
  constexpr size_t NormalSize = 13;
  (hsys != HouseSystem::Gauquelin) ? cusps.resize(NormalSize)
                                   : cusps.resize(GauquelinSize);
  swe_houses_ex(ut, static_cast<std::underlying_type_t<ZodiacType>>(ztype),
                geodetic.lat, geodetic.lon,
                static_cast<std::underlying_type_t<HouseSystem>>(hsys),
                &cusps[0], reinterpret_cast<double *>(&ang));
}
} // namespace swe

} // namespace core
}; // namespace specni

    ; // namespace specni
;     // namespace specni
