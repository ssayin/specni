#include "Chart.hpp"

#include <Combinations.hpp>
#include <cmath>
#include <ranges>

namespace specni::core {
Chart::Chart(const swe::Ut &ut, const swe::Coordinate &geodetic,
             swe::HouseSystem hs, const std::vector<swe::Ipl> &vIpl)
    : houses(ut, geodetic, hs), coord(geodetic), ut(static_cast<double>(ut)) {

  std::vector<swe::Ipl> vIpl2{std::begin(vIpl), std::end(vIpl)};
  std::ranges::sort(vIpl2);

  planets.reserve(vIpl.size());
  std::ranges::transform(vIpl2, std::back_inserter(planets), [&](swe::Ipl ipl) {
    return swe::Planet(ipl, ut, flag);
  });
}

void Chart::update(const swe::Ut &ut, const swe::Coordinate &geodetic,
                   swe::HouseSystem hs) {
  coord = geodetic;
  std::ranges::for_each(planets, [&](swe::Planet &p) { p.update(ut, flag); });

  houses.update(ut, geodetic, hs);
}

} // namespace specni::core
