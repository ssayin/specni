#include "house.h"

// clang-format off
#include "swephexp.h"
#include "swehouse.h"
#include "sweph.h"
// clang-format on

#include <iostream>

int swephpp::houses_ex(double ut, HouseCuspFlag flag, double geolat,
                       double geolon, HouseSystem hsys, Houses &cusps,
                       AscMc &ascmc) {

  return swe_houses_ex(ut, flag, geolat, geolon, hsys,
                reinterpret_cast<double *>(&cusps),
                reinterpret_cast<double *>(&ascmc));
}

const std::string swephpp::house_name(HouseSystem sys) {
  return swe_house_name(static_cast<char>(sys));
}
