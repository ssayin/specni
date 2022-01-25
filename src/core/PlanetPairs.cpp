#include "PlanetPairs.hpp"

#include "Planet.hpp"
#include <cassert>
#include <string>
#include <vector>

namespace specni {
auto GetPlanetCombPairs(std::vector<Planet> &V) -> PlanetPairs {
  PlanetPairs pairs;

  if (V.size() < 2)
    return pairs;

  std::string bitmask(2, 1);
  const auto N = V.size();
  bitmask.resize(N, 0);

  do {
    std::vector<Planet> accum;
    for (std::vector<Planet>::size_type i = 0; i < N; ++i) {
      if (bitmask[i])
        accum.push_back(V[i]);
    }
    assert(accum.size() == 2);
    pairs.push_back({accum[0], accum[1]});
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  return pairs;
}
}; // namespace specni
