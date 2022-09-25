#include "Chart.hpp"

#include <Combinations.hpp>
#include <cmath>

namespace specni::core {
Chart::Chart(const swe::Ut &ut, const swe::Coordinate &geodetic,
             swe::HouseSystem hs, const std::vector<swe::Ipl> &vIpl)
    : houses(ut, geodetic, hs), coord(geodetic), ut(static_cast<double>(ut)) {

  std::vector<swe::Ipl> vIpl2{std::begin(vIpl), std::end(vIpl)};
  std::sort(std::begin(vIpl2), std::end(vIpl2));

  planets.reserve(vIpl.size());
  std::transform(std::cbegin(vIpl2), std::cend(vIpl2),
                 std::back_inserter(planets),
                 [&](swe::Ipl ipl) { return swe::Planet(ipl, ut, flag); });
}

void Chart::update(const swe::Ut &ut, const swe::Coordinate &geodetic,
                   swe::HouseSystem hs) {
  coord = geodetic;
  std::for_each(std::begin(planets), std::end(planets),
                [&](swe::Planet &p) { p.update(ut, flag); });

  houses.update(ut, geodetic, hs);
}

} // namespace specni::core
