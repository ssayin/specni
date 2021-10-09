#pragma once

#include "Planet.hpp"
#include "swephpp.hpp"
#include <array>
#include <core/Cyclic.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

namespace specni {

static const std::array<swephpp::PlanetaryBody, 12> houseOrder /* domicile */ =
    {swephpp::PlanetaryBody::Mars,    swephpp::PlanetaryBody::Venus,
     swephpp::PlanetaryBody::Mercury, swephpp::PlanetaryBody::Moon,
     swephpp::PlanetaryBody::Sun,     swephpp::PlanetaryBody::Mercury,
     swephpp::PlanetaryBody::Venus,   swephpp::PlanetaryBody::Mars,
     swephpp::PlanetaryBody::Jupiter, swephpp::PlanetaryBody::Saturn,
     swephpp::PlanetaryBody::Saturn,  swephpp::PlanetaryBody::Jupiter};

static const std::unordered_map<swephpp::PlanetaryBody, double> exaltations = {
    {swephpp::PlanetaryBody::Sun, 19.0},      // Aries 19
    {swephpp::PlanetaryBody::Moon, 33.0},     // Taurus 3
    {swephpp::PlanetaryBody::TrueNode, 63.0}, // Gemini 3
    {swephpp::PlanetaryBody::Jupiter, 105.0}, // Cancer 15
    {swephpp::PlanetaryBody::Mercury, 195.0}, // Virgo 15
    {swephpp::PlanetaryBody::Saturn, 231.0},  // Libra 21
    {swephpp::PlanetaryBody::Mars, 298.0},    // Aquarius 28
    {swephpp::PlanetaryBody::Venus, 357.0},   // Pisces 27
};

static const std::array<swephpp::PlanetaryBody, 7> faceOrder{
    swephpp::PlanetaryBody::Mars,   swephpp::PlanetaryBody::Sun,
    swephpp::PlanetaryBody::Venus,  swephpp::PlanetaryBody::Mercury,
    swephpp::PlanetaryBody::Moon,   swephpp::PlanetaryBody::Saturn,
    swephpp::PlanetaryBody::Jupiter};

constexpr swephpp::PlanetaryBody face(double lon) {
  return faceOrder.at(std::fmod(lon / 10.0, 7));
}

static const std::unordered_map<swephpp::PlanetaryBody, std::vector<double>>
    terms{
        {swephpp::PlanetaryBody::Mercury,
         {21.0,       26.0,       30 + 15.0,  30 + 22.0,  60 + 7.0,
          60 + 14.0,  90 + 20.0,  90 + 27.0,  120 + 13.0, 120 + 19.0,
          150 + 7.0,  150 + 13.0, 180 + 24.0, 180 + 30.0, 210 + 27.0,
          210 + 30.0, 240 + 19.0, 240 + 25.0, 270 + 12.0, 270 + 19.0,
          300 + 12.0, 300 + 20.0, 330 + 20.0, 330 + 26.0}},

        {swephpp::PlanetaryBody::Venus, {}},

        {swephpp::PlanetaryBody::Mars, {}},

        {swephpp::PlanetaryBody::Jupiter, {}},

        {swephpp::PlanetaryBody::Saturn, {}},

    };

// should also consider mutual reception
inline bool IsDomicile(const Planet &p) {
  return (p.Id == houseOrder[p.Data.lon / 30.0]);
}

inline bool IsInMutualReceptionDomicile(const Planet &p1, const Planet &p2) {
  return IsDomicile(Planet{p1.Id, p2.Data}) &&
         IsDomicile(Planet{p2.Id, p1.Data});
}

// reverse order
inline bool IsInDetriment(const Planet &p) {
  return (p.Id == houseOrder[12 - p.Data.lon / 30.0]);
}

// should consider mutual reception
inline bool IsExalted(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon), 2)) {
      return true;
    }
  }

  return false;
}

inline bool IsMutualReceptionExalted(const Planet &p1, const Planet &p2) {
  return IsExalted(Planet{p1.Id, p2.Data}) && IsExalted(Planet{p2.Id, p1.Data});
}

inline bool IsFallen(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon) - 180.0, 2)) {
      return true;
    }
  }

  return false;
}

enum class EssentialState {
  Domicile,
  InDetriment,
  Exalted,
  Fallen,
  InOwnTriplicity,
  InOwnFace,
  InOwnTerm,
  Peregrine
};

// determine astrological season
// determine whether it is night or day
// get ruler / participitating ruler
inline bool IsInOwnTriplicity(const Planet &p) { return false; }

inline bool IsInOwnFace(const Planet &p) { return (p.Id == face(p.Data.lon)); }

inline bool IsInOwnTerm(const Planet &p) { return true; }

// when a planet lacks any essential dignity, it is peregrine
inline bool IsPeregrine(const Planet &p) { return true; }

// if none of the previous functions return true, then IsPeregrine(...) returns
// true, sequential coupling to prevent mistakes
const std::unordered_map<EssentialState, std::function<bool(const Planet &)>>
    EssentialStateFunctions = {
        {EssentialState::Domicile, IsDomicile},
        {EssentialState::InDetriment, IsInDetriment},
        {EssentialState::Fallen, IsFallen},
        {EssentialState::InOwnTriplicity, IsInOwnTriplicity},
        {EssentialState::InOwnFace, IsInOwnFace},
        {EssentialState::InOwnTerm, IsInOwnTerm},
        {EssentialState::Peregrine, IsPeregrine}};

inline const std::unordered_map<Planet, std::vector<EssentialState>>
GetPlanetEssentialStates(std::vector<Planet> &planets) {
  std::unordered_map<Planet, std::vector<EssentialState>> ret;

  for (auto &p : planets) {
    ret.insert({p, std::vector<EssentialState>()});
    for (auto &ef : EssentialStateFunctions) {
      if (ef.second(p))
        ret.at(p).push_back(ef.first);
    }
  }

  return ret;
}

} // namespace specni
