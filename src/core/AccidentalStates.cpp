#include "AccidentalStates.hpp"
#include "core/MoonPhase.hpp"
#include "core/swephpp.hpp"
#include <exception>

namespace specni {

/*
FIXME: Gauquelin not working
Adapted from Pullen's placalc
*/
int AccidentalStates::GetHouseNum(const Planet &p) {

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

bool AccidentalStates::IsSwift(const Planet &p) {
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

bool AccidentalStates::IsSlow(const Planet &p) {
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

bool AccidentalStates::IsWithinSun(const Planet &p, double deg) {
  return Longitude(GetSun().Data.lon).within(p.Data.lon, deg) &&
         (p.Id != swephpp::PlanetaryBody::Sun);
}

bool AccidentalStates::IsOccidental(const Planet &p) {}

bool AccidentalStates::IsOriental(const Planet &p) {
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

bool AccidentalStates::IsUnderSunBeams(const Planet &p) {
  return IsWithinSun(p, util::dmstodeg(17, 0, 0));
}
bool AccidentalStates::IsCombust(const Planet &p) {
  return IsWithinSun(p, util::dmstodeg(8, 30, 0));
}
bool AccidentalStates::IsCazimi(const Planet &p) {
  return IsWithinSun(p, util::dmstodeg(0, 17, 0));
}

const Planet &AccidentalStates::GetSun() {
  auto it = model.Eph.find(swephpp::PlanetaryBody::Sun);
  if (it != model.Eph.end()) {
    return it->second;
  }

  throw std::exception();
}
}; // namespace specni