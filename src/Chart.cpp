#include "Chart.hpp"
#include "Aspect.hpp"

namespace specni::core {

Chart::Chart(const swe::Ut &ut, const swe::Coordinate &geodetic,
             swe::HouseSystem hs, const std::vector<swe::Ipl> &vIpl)
    : houses(ut, geodetic, hs) {

  constexpr swe::EphFlag flag = swe::EphFlag::SwissEph | swe::EphFlag::Speed;

  for (auto e : vIpl) {
    planets.emplace(swe::Planet{e, ut, flag});
  }
}

} // namespace specni::core
