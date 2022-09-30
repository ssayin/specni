#pragma once

#include <SwEphpp.hpp>
#include <optional>
#include <ranges>
#include <vector>

#include "Combinations.hpp"

namespace specni::core {

template <swe::HasEcliptic EC>
std::optional<uint8_t> maybe_aspect(const EC &ec1, const EC &ec2) {
  static constexpr auto aspects =
      std::to_array<uint8_t>({0, 30, 60, 90, 120, 150, 180});

  auto longitude = [](const EC &ec) { return ec.ecliptic().at(0); };
  auto dif = swe_difdeg2n(longitude(ec1), longitude(ec2));

  auto is_aspect = [&](auto x) { return std::abs(x - dif) <= 10; };

  if (auto it = std::ranges::find_if(aspects, is_aspect);
      it != std::cend(aspects)) {
    return *it;
  } else {
    return std::nullopt;
  }
}

enum { Parallel, Contraparallel };

template <swe::HasEquatorial EQ>
std::optional<uint8_t> maybe_declination_aspect(const EQ &eq1, const EQ &eq2) {
  auto declination = [](const EQ &eq) { return eq.equatorial().at(1); };
  auto xx = declination(eq1);
  auto yy = declination(eq2);

  if (std::abs(xx - yy) <= 1) {
    return Parallel;
  } else if (std::abs(xx + yy) <= 1) {
    return Contraparallel;
  } else {
    return std::nullopt;
  }
}

template <std::ranges::range R, class Func>
void for_each_aspect(R &&r, Func &&f) {
  for_each_combination(
      std::ranges::begin(r), std::ranges::next(std::ranges::begin(r), 2),
      std::ranges::end(r), [&](const auto &x, const auto &y) {
        if (auto it = specni::core::maybe_aspect(*x, *std::ranges::next(x))) {
          f(*x, *std::ranges::next(x), *it);
        }
        return false;
      });
}

class Chart {
public:
  static constexpr swe::EphFlag flag =
      swe::EphFlag::SwissEph | swe::EphFlag::Speed;

  Chart(const swe::Ut &ut, const swe::Coordinate &geodetic, swe::HouseSystem hs,
        const std::vector<swe::Ipl> &vIpl);

  void update(const swe::Ut &ut, const swe::Coordinate &geodetic,
              swe::HouseSystem hs);

  swe::House houses;
  std::vector<swe::Planet> planets;
  swe::Coordinate coord;
  double ut;
};
} // namespace specni::core
