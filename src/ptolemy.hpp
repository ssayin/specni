#pragma once

#include "swephpp.hpp"
#include <array>
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
};

static const std::unordered_map<Aspect, int> aspects = {
    {Conjunction, 0}, {Sextile, 60},     {Square, 90},
    {Trine, 120},     {Opposition, 180},
};

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

} // namespace specni
