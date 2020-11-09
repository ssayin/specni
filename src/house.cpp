#include "house.h"

// clang-format off
extern "C" {
#include "swe/swephexp.h"
#include "swe/swehouse.h"
#include "swe/sweph.h"
}
// clang-format on

#include <iostream>

int swephpp::houses_ex(double ut, HouseCuspFlag flag, double latitude,
                       double longitude, HouseSystem sys) {
  return 0;
}

const std::string swephpp::house_name(HouseSystem sys) {
  return swe_house_name(static_cast<char>(sys));
}
