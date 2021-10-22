#include "EssentialStates.hpp"
#include "core/swephpp.hpp"
#include <algorithm>
#include <core/PlanetPairs.hpp>
#include <tuple>
#include <util/Util.hpp>

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

static std::array<std::array<std::tuple<swephpp::PlanetaryBody, double>, 5>, 12>
    terms{{
        {
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 6.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 14.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 21.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 26.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 30.0),
        },

        {
            std::make_tuple(swephpp::PlanetaryBody::Venus, 8.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 15.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 22.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 26.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 7.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 14.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 21.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 25.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Mars, 6.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 13.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 20.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 27.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 6.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 13.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 19.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 25.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 7.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 13.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 18.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 24.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 7.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 13.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 18.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 24.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Mars, 6.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 14.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 21.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 27.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 8.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 14.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 19.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 25.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Venus, 6.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 12.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 19.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 25.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 6.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 12.0),
            std::make_tuple(swephpp::PlanetaryBody::Venus, 20.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 25.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 30.0),
        },
        {
            std::make_tuple(swephpp::PlanetaryBody::Venus, 8.0),
            std::make_tuple(swephpp::PlanetaryBody::Jupiter, 14.0),
            std::make_tuple(swephpp::PlanetaryBody::Mercury, 20.0),
            std::make_tuple(swephpp::PlanetaryBody::Mars, 26.0),
            std::make_tuple(swephpp::PlanetaryBody::Saturn, 30.0),
        },
    }};

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
GetPlanetEssentialStates(std::vector<Planet> &planets, PlanetPairs &pairs) {
  std::unordered_map<Planet, std::vector<EssentialState>> ret;

  for (auto &p : planets) {
    ret.insert({p, std::vector<EssentialState>()});
    for (auto &ef : EssentialStateFunctions) {
      if (ef.second(p))
        ret.at(p).push_back(ef.first);
    }
  }

  for (std::pair<Planet, Planet> &p : pairs) {

    if (IsMutualReceptionExalted(p.first, p.second)) {
      ret[p.first].push_back(EssentialState::MutualExalted);
      ret[p.second].push_back(EssentialState::MutualExalted);
    }

    if (IsInMutualReceptionDomicile(p.first, p.second)) {
      ret[p.first].push_back(EssentialState::MutualDomicile);
      ret[p.second].push_back(EssentialState::MutualDomicile);
    }
  }

  for (auto &p : planets) {
    if (ret.empty())
      ret[p].push_back(EssentialState::Peregrine);
  }

  return ret;
}

bool IsInOwnTerm(const Planet &p) {
  unsigned sign = p.Data.lon / 30;
  double deg = std::fmod(p.Data.lon, 30.0);
  std::array<std::tuple<swephpp::PlanetaryBody, double>, 5> &a = terms[sign];
  for (std::tuple<swephpp::PlanetaryBody, double> *it = a.begin();
       it != a.end(); it++) {
    if (std::get<1>(*it) <= deg) {
      if (it != a.begin()) {
        return std::get<0>(*(it--)) == p.Id;
      } else
        return std::get<0>(*it) == p.Id;
    }
  }
}
bool IsInOwnTriplicity(const Planet &p) { return false; }
} // namespace specni
