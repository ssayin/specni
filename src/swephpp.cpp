#include "swephpp.h"

// clang-format off
#include "swephexp.h"
#include "sweph.h"
// clang-format on

#include <cstddef>
#include <iostream>

int swephpp::houses_ex(const HousesOpts &opts, swephpp::HouseCusps &cusps,
                       Angles &ascmc) {
  return swe_houses_ex(opts.jd_ut, static_cast<int32_t>(opts.flag), opts.geolat,
                       opts.geolon, static_cast<char>(opts.hsys),
                       reinterpret_cast<double *>(&cusps),
                       reinterpret_cast<double *>(&ascmc));
}

const std::string swephpp::house_name(HouseSystem sys) {
  return swe_house_name(static_cast<char>(sys));
}

int swephpp::calc(const CalcOpts &opts, PlanetEphData &data) {
  return swe_calc_ut(opts.jd_ut, opts.id, static_cast<int32_t>(opts.flag),
                     reinterpret_cast<double *>(&data), 0);
}
