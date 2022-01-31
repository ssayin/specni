#include "PlanetPairs.hpp"

#include "Planet.hpp"
#include <cassert>
#include <string>
#include <vector>

#include <type_traits>
namespace specni {
/*
template <int N, int K, auto T, class C> auto Comb(std::vector<C &> &V) {
  C ret;

  if (ret.size() < K) {
    return ret;
  }

  std::string bitmask(K, 1);
  bitmask.resize(N, 0);

  do {
    std::vector<Planet> accum;
    for (std::vector<Planet>::size_type i = 0; i < N; ++i) {
      if (bitmask[i])
        accum.push_back(V[i]);
    }
    assert(accum.size() == 2);
    ret.push_back({accum[0], accum[1]});
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}
*/
}; // namespace specni
