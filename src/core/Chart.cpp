#include "Chart.hpp"
#include "Aspect.hpp"

#include <tuple>

namespace specni {
namespace core {

auto Combination(std::size_t N) {
  std::string bitmask(2, 1);
  bitmask.resize(N, 0);

  std::vector<std::pair<std::size_t, std::size_t>> r;
  do {
    std::vector<size_t> accum;
    for (std::size_t i = 0u; i < N; ++i) {
      if (bitmask[i])
        accum.push_back(i);
    }
    r.emplace_back(accum[0], accum[1]);

  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

  return r;
}

Chart::Chart(const swe::Ut &ut, const swe::Coordinate &geodetic,
             swe::HouseSystem hs, const std::vector<swe::Ipl> &vIpl)
    : houses(ut, swe::ZodiacType::Tropical, geodetic, hs), planets(ut, vIpl) {

  auto xd = Combination(planets.v.size());

  for (auto xasd : xd) {
    const swe::Planet &p1 = planets.m.at(planets.v[xasd.first]);
    const swe::Planet &p2 = planets.m.at(planets.v[xasd.second]);

    auto asp = aspect::HarmonicBetween<aspect::DefaultOrbConfig>(
        planets.m.at(planets.v[xasd.first]),
        planets.m.at(planets.v[xasd.second]));

    if (asp.has_value()) {
      std::cout << p1.Name() << " " << p2.Name() << " "
                << static_cast<int>(asp->type) * 60 << " "
                << ((asp->detail == aspect::Detail::Applying) ? 'A' : 'S')
                << " " << asp->orb << std::endl;
    }

    auto asp2 = aspect::DeclineBetween<aspect::OrbPartileConfig>(
        planets.m.at(planets.v[xasd.first]),
        planets.m.at(planets.v[xasd.second]));

    if (asp2.has_value()) {
      std::cout << p1.Name() << " " << p2.Name() << " "
                << static_cast<int>(asp2->type) << " "
                << ((asp2->detail == aspect::Detail::Applying) ? 'A' : 'S')
                << " " << asp2->orb << std::endl;
    }
  }
}

}; // namespace core
}; // namespace specni
