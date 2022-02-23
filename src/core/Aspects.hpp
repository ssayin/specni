#pragma once

#include "Cyclic.hpp"
#include "Planet.hpp"
#include "imgui.h"
#include "swephpp.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <core/AspectTypes.hpp>
#include <core/PlanetPairs.hpp>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace specni {

template <class A>
using AspectTuple =
    std::tuple<swephpp::Ipl, swephpp::Ipl, A, double, AspectStat>;

template <class A> using AspectTupleVector = std::vector<AspectTuple<A>>;

template <class A>
constexpr bool IsAspectAngleType = std::is_same<A, AspectAngle>{}();

template <class A>
constexpr bool IsDeclinationType = std::is_same<A, Declination>{}();

template <class A> using AspectFuncRetType = std::tuple<A, double, AspectStat>;

template <class A, class AspectConfig = DefaultOrbConfig>

// this is no longer needed
using AspFuncSignature = std::enable_if_t<
    IsAspectAngleType<A> or IsDeclinationType<A>,
    std::function<AspectFuncRetType<A>(const Planet &, const Planet &)>>;

template <class A, class AspectConfig = DefaultOrbConfig>
auto CalculateAspects(
    PlanetPairs &pairs) //, AspFuncSignature<A, AspectConfig> f)
    -> std::enable_if_t<IsAspectAngleType<A> or IsDeclinationType<A>,
                        AspectTupleVector<A>> {

  AspectTupleVector<A> ret;

  for (std::pair<Planet, Planet> p : pairs) {
    auto asp = AspectFunc<A, AspectConfig>(p.first, p.second);
    if (std::get<0>(asp) != A::Count) {
      ret.push_back({p.first.Id, p.second.Id, std::get<0>(asp),
                     std::get<1>(asp), std::get<2>(asp)});
    }
  }
  return ret;
}

template <typename T, typename U>
using Bigger = typename std::conditional<sizeof(T) >= sizeof(U), T, U>::type;

template <typename T, typename U>
inline auto AddSub(T x, U y) -> std::tuple<Bigger<T, U>, Bigger<T, U>> {
  return {x + y, x - y};
}

template <typename T, typename U = T, typename V = T>
inline auto AddSub(T x, U y, V z) -> auto {
  return std::tuple_cat(AddSub(y - x, z), std::move(AddSub(x - y, z)));
}

static auto MinAddSub(const Longitude &first, const Longitude &second,
                      const Longitude &orb) -> const Longitude {

  const auto &list = std::apply(
      [](auto &&...args) { return std::vector<Longitude>{args...}; },
      std::forward<std::tuple<Longitude, Longitude, Longitude, Longitude>>(
          std::move(AddSub(first, second, orb))));

  return *std::min_element(list.begin(), list.end());
}

// will generate so many functions at compile time, what to do?
template <class A, class AspectConfig>
auto AspectFunc(const Planet &p1, const Planet &p2)
    -> std::enable_if_t<IsDeclinationType<A>, AspectFuncRetType<A>> {
  // This should give ~0 if planets are contra-parallel
  double a, b;
  std::tie(a, b) = AddSub(p1.Data.lat, p2.Data.lat);
  auto next = std::fabs(p1.Data.spdlat + p2.Data.spdlat);
  if (a < AspectConfig::Get(p1.Id)) {
    return std::make_tuple(A::Contraparallel, a,
                           a <= next ? Applying : Seperating);
  } else if (b < AspectConfig::Get(p1.Id)) {
    return std::make_tuple(A::Parallel, b, b <= next ? Applying : Seperating);
  }

  return std::make_tuple(A::Count, 0, AspectStat::Count);
}

template <class A, class AspectConfig>
auto AspectFunc(const Planet &p1, const Planet &p2)
    -> std::enable_if_t<IsAspectAngleType<A>, AspectFuncRetType<A>> {
  Longitude a(p1.Data.lon);
  Longitude b(p2.Data.lon);
  for (auto &asp : Aspects) {
    int maxOrb = std::max(AspectConfig::Get(p1.Id), AspectConfig::Get(p2.Id));
    if (a.within(b + asp.second, maxOrb) || a.within(b - asp.second, maxOrb)) {
      Longitude actualArcDistance = MinAddSub(a, b, asp.second); // implicit

      Longitude predictedArcDistance =
          MinAddSub(a + Longitude(p1.Data.spdlon),
                    b + Longitude(p2.Data.spdlon), asp.second);

      return std::make_tuple(asp.first, actualArcDistance(),
                             ((actualArcDistance() > predictedArcDistance())
                                  ? Applying
                                  : Seperating));
    }
  }
  return std::make_tuple(A::Count, 0, AspectStat::Count);
};
}; // namespace specni