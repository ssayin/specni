#pragma once

#include "swephpp.hpp"
#include <algorithm>
#include <vector>

namespace specni {
struct Planet {
  swephpp::PlanetaryBody Id;
  swephpp::PlanetEphData Data;

  int GetHouseNum(const std::vector<float> &houses, swephpp::HouseSystem hsys) {
    return std::distance(
        houses.begin(),
        std::upper_bound(houses.begin(), houses.end(), Data.lon));
  }
};
}; // namespace specni
