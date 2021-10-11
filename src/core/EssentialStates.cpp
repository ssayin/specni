#include "EssentialStates.hpp"
namespace specni {

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

const std::unordered_map<EssentialState, std::function<bool(const Planet &)>>
    EssentialStateFunctions = {
        {EssentialState::Domicile, IsDomicile},
        {EssentialState::InDetriment, IsInDetriment},
        {EssentialState::Fallen, IsFallen},
        {EssentialState::InOwnTriplicity, IsInOwnTriplicity},
        {EssentialState::InOwnFace, IsInOwnFace},
        {EssentialState::InOwnTerm, IsInOwnTerm}};

// should consider mutual reception
bool IsExalted(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon), 2)) {
      return true;
    }
  }

  return false;
}

bool IsFallen(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon) - 180.0, 2)) {
      return true;
    }
  }

  return false;
}

const std::unordered_map<Planet, std::vector<EssentialState>>
GetPlanetEssentialStates(std::vector<Planet> &planets) {
  std::unordered_map<Planet, std::vector<EssentialState>> ret;

  for (auto &p : planets) {
    ret.insert({p, std::vector<EssentialState>()});
    for (auto &ef : EssentialStateFunctions) {
      if (ef.second(p))
        ret.at(p).push_back(ef.first);
    }
    if (ret.empty())
      ret[p].push_back(EssentialState::Peregrine);
  }

  return ret;
}

} // namespace specni
