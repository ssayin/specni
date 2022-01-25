#include "PlanetStates.hpp"
#include "core/Cyclic.hpp"
#include "core/MoonPhase.hpp"
#include "core/swephpp.hpp"
#include <exception>

namespace specni {

/*
FIXME: Gauquelin not working
Adapted from Pullen's placalc
*/
auto PlanetStates::GetHouseNum(const Planet &p) -> int {

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

auto PlanetStates::IsSwift(const Planet &p) -> bool {
  float spd = fabs(p.Data.spdlon);
  switch (p.Id) {
  case swephpp::Ipl::Moon:
    return spd >= 13.30;
  case swephpp::Ipl::Mercury:
    return spd >= 1.38;
  case swephpp::Ipl::Venus:
    return spd >= 1.20;
  case swephpp::Ipl::Mars:
    return spd >= 0.40;
  case swephpp::Ipl::Jupiter:
    return spd >= 0.10;
  case swephpp::Ipl::Saturn:
    return spd >= 0.05;
  default:
    return false;
  }
};

auto PlanetStates::IsSlow(const Planet &p) -> bool {
  float spd = fabs(p.Data.spdlon);
  switch (p.Id) {
  case swephpp::Ipl::Moon:
    return spd <= 12.30;
  case swephpp::Ipl::Mercury:
    return spd <= 1.00;
  case swephpp::Ipl::Venus:
    return spd <= 0.50;
  case swephpp::Ipl::Mars:
    return spd <= 0.30;
  case swephpp::Ipl::Jupiter:
    return spd <= 0.05;
  case swephpp::Ipl::Saturn:
    return spd <= 0.02;
  default:
    return false;
  }
}

auto PlanetStates::IsWithinSun(const Planet &p, double deg) -> bool {
  return Longitude(GetSun().Data.lon).within(p.Data.lon, deg) &&
         (p.Id != swephpp::Ipl::Sun);
}

auto PlanetStates::IsOccidental(const Planet &p) -> bool { return !IsOriental(p); }

auto PlanetStates::IsOriental(const Planet &p) -> bool {
  double tret[3];
  double tret2[3];
  char err[256];
  double atPressure = 0;
  double atTemp = 0;

  double geo[3] = {model.geolon, model.geolat, 0};

  swe_rise_trans(model.ut, static_cast<int32>(p.Id), nullptr, 0, SE_CALC_RISE, geo,
                 atPressure, atTemp, tret, err);

  swe_rise_trans(model.ut, static_cast<int32>(swephpp::Ipl::Sun), nullptr, 0,
                 SE_CALC_RISE, geo, atPressure, atTemp, tret2, err);

  return tret[0] < tret2[0];
}

auto PlanetStates::IsUnderSunBeams(const Planet &p) -> bool {
  return IsWithinSun(p, util::dmstodeg(17, 0, 0));
}
auto PlanetStates::IsCombust(const Planet &p) -> bool {
  return IsWithinSun(p, util::dmstodeg(8, 30, 0));
}
auto PlanetStates::IsCazimi(const Planet &p) -> bool {
  return IsWithinSun(p, util::dmstodeg(0, 17, 0));
}

auto PlanetStates::GetSun() -> const Planet & { return GetPlanet(swephpp::Ipl::Sun); }

static const std::unordered_map<swephpp::Ipl, double> exaltations = {
    {swephpp::Ipl::Sun, 19.0},      // Aries 19
    {swephpp::Ipl::Moon, 33.0},     // Taurus 3
    {swephpp::Ipl::TrueNode, 63.0}, // Gemini 3
    {swephpp::Ipl::Jupiter, 105.0}, // Cancer 15
    {swephpp::Ipl::Mercury, 195.0}, // Virgo 15
    {swephpp::Ipl::Saturn, 231.0},  // Libra 21
    {swephpp::Ipl::Mars, 298.0},    // Aquarius 28
    {swephpp::Ipl::Venus, 357.0},   // Pisces 27
};

static std::array<std::array<std::tuple<swephpp::Ipl, double>, 5>, 12> terms{{
    {
        std::make_tuple(swephpp::Ipl::Jupiter, 6.0),
        std::make_tuple(swephpp::Ipl::Venus, 14.0),
        std::make_tuple(swephpp::Ipl::Mercury, 21.0),
        std::make_tuple(swephpp::Ipl::Mars, 26.0),
        std::make_tuple(swephpp::Ipl::Saturn, 30.0),
    },

    {
        std::make_tuple(swephpp::Ipl::Venus, 8.0),
        std::make_tuple(swephpp::Ipl::Mercury, 15.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 22.0),
        std::make_tuple(swephpp::Ipl::Saturn, 26.0),
        std::make_tuple(swephpp::Ipl::Mars, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Mercury, 7.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 14.0),
        std::make_tuple(swephpp::Ipl::Venus, 21.0),
        std::make_tuple(swephpp::Ipl::Saturn, 25.0),
        std::make_tuple(swephpp::Ipl::Mars, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Mars, 6.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 13.0),
        std::make_tuple(swephpp::Ipl::Mercury, 20.0),
        std::make_tuple(swephpp::Ipl::Venus, 27.0),
        std::make_tuple(swephpp::Ipl::Saturn, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Saturn, 6.0),
        std::make_tuple(swephpp::Ipl::Mercury, 13.0),
        std::make_tuple(swephpp::Ipl::Venus, 19.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 25.0),
        std::make_tuple(swephpp::Ipl::Mars, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Mercury, 7.0),
        std::make_tuple(swephpp::Ipl::Venus, 13.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 18.0),
        std::make_tuple(swephpp::Ipl::Saturn, 24.0),
        std::make_tuple(swephpp::Ipl::Mars, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Saturn, 7.0),
        std::make_tuple(swephpp::Ipl::Venus, 13.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 18.0),
        std::make_tuple(swephpp::Ipl::Mercury, 24.0),
        std::make_tuple(swephpp::Ipl::Mars, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Mars, 6.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 14.0),
        std::make_tuple(swephpp::Ipl::Venus, 21.0),
        std::make_tuple(swephpp::Ipl::Mercury, 27.0),
        std::make_tuple(swephpp::Ipl::Saturn, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Jupiter, 8.0),
        std::make_tuple(swephpp::Ipl::Venus, 14.0),
        std::make_tuple(swephpp::Ipl::Mercury, 19.0),
        std::make_tuple(swephpp::Ipl::Saturn, 25.0),
        std::make_tuple(swephpp::Ipl::Mars, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Venus, 6.0),
        std::make_tuple(swephpp::Ipl::Mercury, 12.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 19.0),
        std::make_tuple(swephpp::Ipl::Mars, 25.0),
        std::make_tuple(swephpp::Ipl::Saturn, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Saturn, 6.0),
        std::make_tuple(swephpp::Ipl::Mercury, 12.0),
        std::make_tuple(swephpp::Ipl::Venus, 20.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 25.0),
        std::make_tuple(swephpp::Ipl::Mars, 30.0),
    },
    {
        std::make_tuple(swephpp::Ipl::Venus, 8.0),
        std::make_tuple(swephpp::Ipl::Jupiter, 14.0),
        std::make_tuple(swephpp::Ipl::Mercury, 20.0),
        std::make_tuple(swephpp::Ipl::Mars, 26.0),
        std::make_tuple(swephpp::Ipl::Saturn, 30.0),
    },
}};

// should consider mutual reception
auto PlanetStates::IsExalted(const Planet &p) -> bool {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon), 2)) {
      return true;
    }
  }

  return false;
}

auto PlanetStates::IsFallen(const Planet &p) -> bool {
  auto it = exaltations.find(p.Id);
  if (it != exaltations.end()) {
    if (Longitude(it->second).within(Longitude(p.Data.lon) - 180.0, 2)) {
      return true;
    }
  }

  return false;
}

using PlanetEStateMap = std::unordered_map<Planet, std::vector<EssentialState>>;

auto PlanetStates::GetPlanetEssentialStates() -> const PlanetEStateMap {
  PlanetEStateMap ret;

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

    if (ret[p.second].empty())
      ret[p.second].push_back(EssentialState::Peregrine);
  }

  return ret;
}

auto PlanetStates::IsInOwnTerm(const Planet &p) -> bool {
  auto sign = static_cast<unsigned>(p.Data.lon / 30);
  double deg = std::fmod(p.Data.lon, 30.0);
  std::array<std::tuple<swephpp::Ipl, double>, 5> &a = terms[sign];
  for (std::tuple<swephpp::Ipl, double> *it = a.begin(); it != a.end(); it++) {
    if (std::get<1>(*it) <= deg) {
      if (it != a.begin()) {
        return std::get<0>(*(it--)) == p.Id;
      } else
        return std::get<0>(*it) == p.Id;
    }
  }
  return false;
}

auto PlanetStates::IsItNight() -> bool {
  const float HalfCircle = 180.0;

  double lon = GetSun().Data.lon;
  double tmp = model.ascmc.ac;

  if (tmp >= HalfCircle) {
    tmp -= 180.0;
    lon = (Longitude(lon) - Longitude(180))();
  }

  return (lon >= tmp && lon <= (tmp + 180.0));
}

auto PlanetStates::GetPlanet(swephpp::Ipl id) -> const Planet & {
  auto it = model.Eph.find(id);
  if (it != model.Eph.end()) {
    return it->second;
  }

  throw std::exception();
}

}; // namespace specni