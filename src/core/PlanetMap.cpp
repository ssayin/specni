#include "PlanetMap.hpp"

#include "Aspect.hpp"

namespace specni {
namespace core {

PlanetMap::PlanetMap(const swe::Ut &ut, const std::vector<swe::Ipl> &vIpl)
    : v(vIpl) {
  constexpr swe::EphFlag flag =
      swe::EphFlag::SwissEph | swe::EphFlag::Speed | swe::EphFlag::Equatorial;

  for (auto e : this->v) {
    m.insert({e, swe::Planet{e, ut, flag}});
  }
}

}; // namespace core
}; // namespace specni
