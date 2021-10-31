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
  int GetHouseNum(const Planet &p);

  bool IsSwift(const Planet &);
  bool IsSlow(const Planet &);

  inline bool IsRetrograde(const Planet &p) { return (p.Data.spdlon < 0.0); }
  inline bool IsDirect(const Planet &p) { return !IsRetrograde(p); }

  bool IsOriental(const Planet &);
  bool IsOccidental(const Planet &);
  bool IsUnderSunBeams(const Planet &);
  bool IsCombust(const Planet &);
  bool IsCazimi(const Planet &);

  constexpr swephpp::Ipl face(double lon) {
    return faceOrder.at(std::fmod(lon / 10.0, 7));
  }

  // should also consider mutual reception
  inline bool IsDomicile(const Planet &p) {
    return (p.Id == houseOrder[static_cast<int>(p.Data.lon / 30)]);
  }

  inline bool IsInMutualReceptionDomicile(const Planet &p1, const Planet &p2) {
    return IsDomicile(Planet{p1.Id, p2.Data}) &&
           IsDomicile(Planet{p2.Id, p1.Data});
  }

  inline bool IsInDetriment(const Planet &p) {
    return (p.Id ==
            houseOrder[static_cast<int>((6 + (p.Data.lon / 30.0))) % 12]);
  }

  /*
    Astrological night begins when sun is below Descendant
  */
  bool IsItNight();

  inline bool IsInOwnTriplicity(const Planet &p) {
    const int sign = static_cast<int>(p.Data.lon / 30.0);
    auto &rulers = triplicityRulers[sign];
    return (IsItNight() ? std::get<1>(rulers) : std::get<0>(rulers)) == p.Id;
  }

  // should consider mutual reception
  bool IsExalted(const Planet &p);

  bool IsFallen(const Planet &p);

  inline bool IsMutualReceptionExalted(const Planet &p1, const Planet &p2) {
    return IsExalted(Planet{p1.Id, p2.Data}) &&
           IsExalted(Planet{p2.Id, p1.Data});
  }

  inline bool IsInOwnFace(const Planet &p) {
    return (p.Id == face(p.Data.lon));
  }

  bool IsInOwnTerm(const Planet &p);

  const std::unordered_map<Planet, std::vector<EssentialState>>
  GetPlanetEssentialStates();
  const Planet &GetPlanet(swephpp::Ipl id);
  const Planet &GetSun();

private:
  const ChartModel &model;
  bool IsWithinSun(const Planet &p, double deg);
};

} // namespace specni