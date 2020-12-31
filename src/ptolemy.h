#include "swephpp.h"
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
         {
             21.0,
         }},
    };

} // namespace specni
