#pragma once

#include "core/ChartModel.hpp"
#include <algorithm>
#include <core/Cyclic.hpp>
#include <core/Dignities.hpp>
#include <core/Planet.hpp>
#include <core/swephpp.hpp>
#include <cstdlib>
#include <unordered_map>
#include <vector>

namespace specni {

class PlanetStates {
public:
  PlanetStates(const ChartModel &model) : model(model) {}
  auto GetHouseNum(const Planet &p) -> int;

  auto IsSwift(const Planet &) -> bool;
  auto IsSlow(const Planet &) -> bool;

  inline auto IsRetrograde(const Planet &p) -> bool {
    return (p.Data.spdlon < 0.0);
  }
  inline auto IsDirect(const Planet &p) -> bool { return !IsRetrograde(p); }

  auto IsOriental(const Planet &) -> bool;
  auto IsOccidental(const Planet &) -> bool;
  auto IsUnderSunBeams(const Planet &) -> bool;
  auto IsCombust(const Planet &) -> bool;
  auto IsCazimi(const Planet &) -> bool;

  constexpr auto face(double lon) -> swephpp::Ipl {
    return faceOrder.at(std::fmod(lon / 10.0, 7));
  }

  // should also consider mutual reception
  inline auto IsDomicile(const Planet &p) -> bool {
    return (p.Id == houseOrder[static_cast<int>(p.Data.lon / 30)]);
  }

  inline auto IsInMutualReceptionDomicile(const Planet &p1, const Planet &p2)
      -> bool {
    return IsDomicile(Planet{p1.Id, p2.Data}) &&
           IsDomicile(Planet{p2.Id, p1.Data});
  }

  inline auto IsInDetriment(const Planet &p) -> bool {
    return (p.Id ==
            houseOrder[static_cast<int>((6 + (p.Data.lon / 30.0))) % 12]);
  }

  /*
    Astrological night begins when sun is below Descendant
  */
  auto IsItNight() -> bool;

  inline auto IsInOwnTriplicity(const Planet &p) -> bool {
    const int sign = static_cast<int>(p.Data.lon / 30.0);
    auto &rulers = triplicityRulers[sign];
    return (IsItNight() ? std::get<1>(rulers) : std::get<0>(rulers)) == p.Id;
  }

  // should consider mutual reception
  auto IsExalted(const Planet &p) -> bool;

  auto IsFallen(const Planet &p) -> bool;

  inline auto IsMutualReceptionExalted(const Planet &p1, const Planet &p2)
      -> bool {
    return IsExalted(Planet{p1.Id, p2.Data}) &&
           IsExalted(Planet{p2.Id, p1.Data});
  }

  inline auto IsInOwnFace(const Planet &p) -> bool {
    return (p.Id == face(p.Data.lon));
  }

  auto IsInOwnTerm(const Planet &p) -> bool;

  auto GetPlanetEssentialStates()
      -> const std::unordered_map<Planet, std::vector<EssentialState>>;
  auto GetPlanet(swephpp::Ipl id) -> const Planet &;
  auto GetSun() -> const Planet &;

private:
  const ChartModel &model;
  auto IsWithinSun(const Planet &p, double deg) -> bool;
};

} // namespace specni