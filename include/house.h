#pragma once

#include <cstdint>
#include <string>

namespace swephpp {

enum HouseCuspFlag : int32_t { Tropical = 0, Sidereal, Radians };
enum HouseSystem {
  Equal = 'A',
  Alcabitius,
};

typedef struct {
  float cusp[37];
} Houses;

typedef struct {
  double ascendant;
  double mc;
  double armc;
  double vertex;
  double equasc; /* equatorial ascendant */
  double coasc1; /* co-ascendant (W. Koch) */
  double coasc2; /* co-ascendant (M. Munkasey)*/
  double polasc; /* polar ascendant (M. Munkasey)*/
} AscMc;

int houses_ex(double jd_ut, HouseCuspFlag flag, double geolat, double geolon,
              HouseSystem sys, Houses &cusps, AscMc &ascmc);

const std::string house_name(HouseSystem sys);

} // namespace swephpp
