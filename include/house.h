#pragma once

#include <string>

namespace swephpp {

enum HouseCuspFlag { Tropical = 0, Sidereal, Radians };
enum HouseSystem {
  Equal = 'A',
  Alcabitius = 'B',
};

int houses_ex(double ut, HouseCuspFlag flag, double latitude, double longitude,
              HouseSystem sys);

const std::string house_name(HouseSystem sys);

} // namespace swephpp
