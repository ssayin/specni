#pragma once

#include "Planet.hpp"
#include "swephpp.hpp"
#include <array>
#include <functional>
#include <include/Cyclic.hpp>
#include <unordered_map>
#include <vector>

namespace specni {

static const std::array<swephpp::PlanetaryBody, 12> houseOrder /* domicile */ =
    {swephpp::Mars,    swephpp::Venus,   swephpp::Mercury, swephpp::Moon,
     swephpp::Sun,     swephpp::Mercury, swephpp::Venus,   swephpp::Mars,
     swephpp::Jupiter, swephpp::Saturn,  swephpp::Saturn,  swephpp::Jupiter};

static const std::unordered_map<swephpp::PlanetaryBody, double> exaltations = {
    {swephpp::Sun, 19.0},      // Aries 19
    {swephpp::Moon, 33.0},     // Taurus 3
    {swephpp::TrueNode, 63.0}, // Gemini 3
    {swephpp::Jupiter, 105.0}, // Cancer 15
    {swephpp::Mercury, 195.0}, // Virgo 15
    {swephpp::Saturn, 231.0},  // Libra 21
    {swephpp::Mars, 298.0},    // Aquarius 28
    {swephpp::Venus, 357.0},   // Pisces 27
};

static const std::array<swephpp::PlanetaryBody, 7> faceOrder{
    swephpp::Mars, swephpp::Sun,    swephpp::Venus,  swephpp::Mercury,
    swephpp::Moon, swephpp::Saturn, swephpp::Jupiter};

constexpr swephpp::PlanetaryBody face(double lon) {
  return faceOrder.at(std::fmod(lon / 10.0, 7));
}

static const std::unordered_map<swephpp::PlanetaryBody, std::vector<double>>
    terms{
        {swephpp::Mercury,
         {21.0,       26.0,       30 + 15.0,  30 + 22.0,  60 + 7.0,
          60 + 14.0,  90 + 20.0,  90 + 27.0,  120 + 13.0, 120 + 19.0,
          150 + 7.0,  150 + 13.0, 180 + 24.0, 180 + 30.0, 210 + 27.0,
          210 + 30.0, 240 + 19.0, 240 + 25.0, 270 + 12.0, 270 + 19.0,
          300 + 12.0, 300 + 20.0, 330 + 20.0, 330 + 26.0}},

        {swephpp::Venus, {}},

        {swephpp::Mars, {}},

        {swephpp::Jupiter, {}},

        {swephpp::Saturn, {}},

    };

enum EssentialState {
  Domicile,
  InDetriment,
  Exalted,
  Fallen,
  InOwnTriplicity,
  InOwnFace,
  InOwnTerm,
  Peregrine
};

// should also consider mutual reception
inline bool IsDomicile(const Planet &p) {
  return (p.Id == houseOrder[p.Data.lon / 30.0]);
}

// reverse order
inline bool IsInDetriment(const Planet &p) {
  return (p.Id == houseOrder[12 - p.Data.lon / 30.0]);
}

// should consider mutual reception
inline bool IsExalted(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    Longitude lon = it->second;
    Longitude lon2 = p.Data.lon;
    if (lon.within(lon2, 2)) {
      return true;
    }
  }

  return false;
}

inline bool IsFallen(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    Longitude lon = it->second - 180.0;
    Longitude lon2 = p.Data.lon;
    if (lon.within(lon2, 2)) {
      return true;
    }
  }

  return false;
}

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
const std::array<std::function<bool(const Planet &)>, 8>
    EssentialStateFunctions = {IsDomicile,  IsInDetriment,     IsExalted,
                               IsFallen,    IsInOwnTriplicity, IsInOwnFace,
                               IsInOwnTerm, IsPeregrine};

} // namespace specni
