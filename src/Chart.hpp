#pragma once

#include <SwEphpp.hpp>
#include <optional>
#include <vector>

namespace specni::core {

template <swe::HasEcliptic EC>
std::optional<uint8_t> maybe_aspect(const EC &ec1, const EC &ec2) {
  static constexpr auto aspects =
      std::to_array<uint8_t>({0, 30, 60, 90, 120, 150, 180});

  auto longitude = [](const EC &ec) { return ec.ecliptic().at(0); };
  auto dif = swe_difdeg2n(longitude(ec1), longitude(ec2));

  auto is_aspect = [&](auto x) { return std::abs(x - dif) <= 10; };

  auto it = std::find_if(std::cbegin(aspects), std::cend(aspects), is_aspect);
  if (it != std::cend(aspects)) {
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

class Chart {
public:
  static constexpr swe::EphFlag flag =
      swe::EphFlag::SwissEph | swe::EphFlag::Speed;

  Chart(const swe::Ut &ut, const swe::Coordinate &geodetic, swe::HouseSystem hs,
        const std::vector<swe::Ipl> &vIpl);

  void update(const swe::Ut &ut, const swe::Coordinate &geodetic,
              swe::HouseSystem hs);

  const auto planetsBegin() { return planets.cbegin(); }
  const auto planetsEnd() { return planets.cend(); }

  swe::House houses;
  std::vector<swe::Planet> planets;
  swe::Coordinate coord;
  double ut;
};
} // namespace specni::core
