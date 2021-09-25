#pragma once

#include "swephpp.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace specni {
/*
 * Only major aspects for now
 */

enum Aspect {
  Conjunction,
  Sextile,
  Square,
  Trine,
  Opposition,
  None,
};

const std::unordered_map<Aspect, int> Aspects = {
    {Conjunction, 0}, {Sextile, 60},     {Square, 90},
    {Trine, 120},     {Opposition, 180},
};

typedef std::vector<
    std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody, Aspect>>
    AspectMatrix;

template <class Func>
AspectMatrix CalculateAspects(std::vector<swephpp::PlanetEphData> V, Func f) {
  std::string bitmask(2, 1);
  const auto N = V.size();
  bitmask.resize(N, 0);

  AspectMatrix ret;

  do {
    std::vector<std::pair<swephpp::PlanetEphData, int>> accum;
    for (int i = 0; i < N; ++i) {
      if (bitmask[i])
        accum.push_back({V[i], i});
    }
    assert(accum.size() == 2);
    swephpp::PlanetEphData &first = std::get<0>(accum[0]);
    swephpp::PlanetEphData &second = std::get<0>(accum[1]);

    Aspect asp = f(first, second);
    if (asp != Aspect::None) {
      ret.push_back({static_cast<swephpp::PlanetaryBody>(std::get<1>(accum[0])),
                     static_cast<swephpp::PlanetaryBody>(std::get<1>(accum[1])),
                     f(first, second)});
    }
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  return ret;
}

}; // namespace specni