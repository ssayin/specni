#include "PlanetStates.hpp"
#include "core/MoonPhase.hpp"
#include "core/swephpp.hpp"
#include <exception>

namespace specni {

/*
FIXME: Gauquelin not working
Adapted from Pullen's placalc
*/
int PlanetStates::GetHouseNum(const Planet &p) {

  const auto &houses = model.vHouseCusps;
  const float epsilon = 1.7453E-09;
  int i = 0;
  Longitude lon = Longitude(p.Data.lon) + epsilon;
  std::vector<float>::size_type houseSize = houses.size();
  if (model.hsys != swephpp::HouseSystem::Gauquelin) {
    while (i < houseSize &&
           !(lon >= houses[i] && lon < houses[(i + 1) % houseSize]) &&
           (houses[i] <= houses[(i + 1) % houseSize] ||
            (lon < houses[i] && lon >= houses[(i + 1) % houseSize])))
      i++;
  }

  i++;
  return i;
}

bool PlanetStates::IsSwift(const Planet &p) {
  float spd = fabs(p.Data.spdlon);
  switch (p.Id) {
  case swephpp::PlanetaryBody::Moon:
    return spd >= 13.30;
  case swephpp::PlanetaryBody::Mercury:
    return spd >= 1.38;
  case swephpp::PlanetaryBody::Venus:
    return spd >= 1.20;
  case swephpp::PlanetaryBody::Mars:
    return spd >= 0.40;
  case swephpp::PlanetaryBody::Jupiter:
    return spd >= 0.10;
  case swephpp::PlanetaryBody::Saturn:
    return spd >= 0.05;
  default:
    return false;
  }
};

bool PlanetStates::IsSlow(const Planet &p) {
  float spd = fabs(p.Data.spdlon);
  switch (p.Id) {
  case swephpp::PlanetaryBody::Moon:
    return spd <= 12.30;
  case swephpp::PlanetaryBody::Mercury:
    return spd <= 1.00;
  case swephpp::PlanetaryBody::Venus:
    return spd <= 0.50;
  case swephpp::PlanetaryBody::Mars:
    return spd <= 0.30;
  case swephpp::PlanetaryBody::Jupiter:
    return spd <= 0.05;
  case swephpp::PlanetaryBody::Saturn:
    return spd <= 0.02;
  default:
    return false;
  }
}

bool PlanetStates::IsWithinSun(const Planet &p, double deg) {
  return Longitude(GetSun().Data.lon).within(p.Data.lon, deg) &&
         (p.Id != swephpp::PlanetaryBody::Sun);
}

bool PlanetStates::IsOccidental(const Planet &p) {}

bool PlanetStates::IsOriental(const Planet &p) {
  auto phase = model.phase;
  if (p.Id == swephpp::PlanetaryBody::Moon) {
    if (phase == MoonPhase::NewMoon || phase == MoonPhase::WaxingCrescent ||
        phase == MoonPhase::FirstQuarter || phase == MoonPhase::WaxingGibbous ||
        phase == MoonPhase::FullMoon) {
      return true;
    }
  }

  if (p.Id == swephpp::PlanetaryBody::Sun) {
  }
}

bool PlanetStates::IsUnderSunBeams(const Planet &p) {
  return IsWithinSun(p, util::dmstodeg(17, 0, 0));
}
bool PlanetStates::IsCombust(const Planet &p) {
  return IsWithinSun(p, util::dmstodeg(8, 30, 0));
}
bool PlanetStates::IsCazimi(const Planet &p) {
  return IsWithinSun(p, util::dmstodeg(0, 17, 0));
}

const Planet &PlanetStates::GetSun() {
  auto it = model.Eph.find(swephpp::PlanetaryBody::Sun);
  if (it != model.Eph.end()) {
    return it->second;
  }

  throw std::exception();
}

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

// should consider mutual reception
bool PlanetStates::IsExalted(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon), 2)) {
      return true;
    }
  }

  return false;
}

bool PlanetStates::IsFallen(const Planet &p) {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon) - 180.0, 2)) {
      return true;
    }
  }

  return false;
}

const std::unordered_map<Planet, std::vector<EssentialState>>
PlanetStates::GetPlanetEssentialStates() {
  std::unordered_map<Planet, std::vector<EssentialState>> ret;

  for (auto &p : model.Eph) {
    ret.insert({p.second, std::vector<EssentialState>()});

    if (IsDomicile(p.second)) {
      ret[p.second].push_back(EssentialState::Domicile);
    }
    if (IsInDetriment(p.second)) {
      ret[p.second].push_back(EssentialState::InDetriment);
    }

    if (IsExalted(p.second)) {
      ret[p.second].push_back(EssentialState::Exalted);
    }

    if (IsFallen(p.second)) {
      ret[p.second].push_back(EssentialState::Fallen);
    }
    if (IsInOwnFace(p.second)) {
      ret[p.second].push_back(EssentialState::InOwnFace);
    }

    if (IsInOwnTerm(p.second)) {
      ret[p.second].push_back(EssentialState::InOwnTerm);
    }

    if (IsInOwnTriplicity(p.second)) {
      ret.at(p.second).push_back(EssentialState::InOwnTriplicity);
    }
  }

  for (std::pair<Planet, Planet> p : model.pairs) {

    if (IsMutualReceptionExalted(p.first, p.second)) {
      ret[p.first].push_back(EssentialState::MutualExalted);
      ret[p.second].push_back(EssentialState::MutualExalted);
    }

    if (IsInMutualReceptionDomicile(p.first, p.second)) {
      ret[p.first].push_back(EssentialState::MutualDomicile);
      ret[p.second].push_back(EssentialState::MutualDomicile);
    }
  }

  for (auto &p : model.Eph) {
    if (ret[p.second].empty())
      ret[p.second].push_back(EssentialState::Peregrine);
  }

  return ret;
}

bool PlanetStates::IsInOwnTerm(const Planet &p) {
  unsigned sign = static_cast<unsigned>(p.Data.lon / 30);
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
  return false;
}

bool PlanetStates::IsItNight() {
  const float HalfCircle = 180.0;

  double lon = GetSun().Data.lon;
  double tmp = model.ascmc.ac;

  if (tmp >= HalfCircle) {
    tmp -= 180.0;
    lon = (Longitude(lon) - Longitude(180))();
  }

  return (lon >= tmp && lon <= (tmp + 180.0));
}
}; // namespace specni