#pragma once

#include "Planet.hpp"
#include "PlanetPairs.hpp"
#include "swephpp.hpp"
#include <array>
#include <cassert>
#include <core/ChartModel.hpp>
#include <core/Cyclic.hpp>
#include <core/Dignities.hpp>
#include <core/EssentialState.hpp>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace specni {

constexpr swephpp::PlanetaryBody face(double lon) {
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
  return (p.Id == houseOrder[static_cast<int>((6 + (p.Data.lon / 30.0))) % 12]);
}

/*
  Astrological night begins when sun is below Descendant
*/
bool IsItNight(const ChartModel &model);

inline bool IsInOwnTriplicity(const Planet &p, const ChartModel &model) {
  const int sign = static_cast<int>(p.Data.lon / 30.0);
  auto &rulers = triplicityRulers[sign];
  return (IsItNight(model) ? std::get<1>(rulers) : std::get<0>(rulers)) == p.Id;
}

// should consider mutual reception
bool IsExalted(const Planet &p);

bool IsFallen(const Planet &p);

inline bool IsMutualReceptionExalted(const Planet &p1, const Planet &p2) {
  return IsExalted(Planet{p1.Id, p2.Data}) && IsExalted(Planet{p2.Id, p1.Data});
}

inline bool IsInOwnFace(const Planet &p) { return (p.Id == face(p.Data.lon)); }

bool IsInOwnTerm(const Planet &p);

const std::unordered_map<Planet, std::vector<EssentialState>>
GetPlanetEssentialStates(const ChartModel &model);

} // namespace specni
