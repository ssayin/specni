#pragma once

#include "Cyclic.hpp"
#include "Planet.hpp"
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

struct OrbConfig {
  static constexpr int Get(swephpp::PlanetaryBody body) {
    switch (body) {
    case swephpp::Sun:
      return 7;
    case swephpp::Moon:
      return 6;
    case swephpp::Mercury:
    case swephpp::Venus:
      return 3;
    case swephpp::Mars:
      return 4;
    case swephpp::Jupiter:
    case swephpp::Saturn:
      return 5;
    default:
      return 2;
    }
  }
};

typedef std::vector<
    std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody, Aspect>>
    AspectMatrix;

template <class Func>
AspectMatrix CalculateAspects(std::vector<Planet> V, Func f) {
  AspectMatrix ret;

  if (V.size() < 2)
    return ret;

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
    Planet &first = accum[0];
    Planet &second = accum[1];

    Aspect asp = f(first, second);
    if (asp != Aspect::None) {
      ret.push_back({first.Id, second.Id, f(first, second)});
    }
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  return ret;
}

template <class Config = OrbConfig>
auto AspectFunc = [](const Planet &p1, const Planet &p2) -> Aspect {
  auto a = Longitude(p1.Data.lon);
  auto b = Longitude(p2.Data.lon);
  for (auto &p : Aspects) {
    auto left = b + p.second;
    auto right = b - p.second;
    int maxOrb = std::max(Config::Get(p1.Id), Config::Get(p2.Id));
    if (a.within(left, maxOrb) || a.within(right, maxOrb)) {
      return p.first;
    }
  }
  return Aspect::None;
};

}; // namespace specni