#include "AccidentalStates.hpp"

namespace specni {

float MinDifference(float deg1, float deg2) {
  float r;

  r = deg2 - deg1;
  if (fabs(r) < 180.0)
    return r;

  return r >= 0 ? r - 360.0 : r + 360.0;
}

/*
FIXME: Gauquelin not working
Adapted from Pullen's placalc
*/
int GetHouseNum(const Planet &p, const std::vector<float> &houses,
                swephpp::HouseSystem hsys) {

  const float epsilon = 1.7453E-09;
  int i = 0;
  Longitude lon = Longitude(p.Data.lon) + epsilon;
  std::vector<float>::size_type houseSize = houses.size();
  if (hsys != swephpp::HouseSystem::Gauquelin) {
    while (i < houseSize &&
           !(lon >= houses[i] && lon < houses[(i + 1) % houseSize]) &&
           (houses[i] <= houses[(i + 1) % houseSize] ||
            (lon < houses[i] && lon >= houses[(i + 1) % houseSize])))
      i++;
  }

  i++;
  return i;
}

bool IsSwift(const Planet &p) {
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

bool IsSlow(const Planet &p) {
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

}; // namespace specni