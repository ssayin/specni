#pragma once

#include "sweodef.h"
#include <core/Aspects.hpp>
#include <core/EssentialState.hpp>
#include <core/MoonPhase.hpp>
#include <core/Planet.hpp>
#include <core/PlanetPairs.hpp>
#include <core/swephpp.hpp>
#include <string>
#include <unordered_map>
#include <util/Util.hpp>
#include <vector>

namespace specni {

struct ChartModel {
public:
  ChartModel() = default;
  ~ChartModel() = default;

  void SetDateGregorian(int year, int month, int day, int hour, int min,
                        double sec) {
    double dret[2];
    char serr[256];
    assert(swe_utc_to_jd(year, month, day, hour, min, sec, 1, dret, serr) ==
           OK);
    this->ut = dret[1]; // UT1
  }

  void SetCoordinates(double latitude, double longitude) {
    geolat = latitude;
    geolon = longitude;
  }

  void SetHouseSystem(swephpp::HouseSystem sys) { hsys = sys; }

  void RecalculateAspects();

  void RecalculatePlanetPos();

  void RecalculateHouses();

  void GetStars();

  swephpp::HouseSystem hsys;
  swephpp::Angles ascmc;
  std::vector<float> vHouseCusps;
  std::unordered_map<swephpp::Ipl, Planet> Eph;

  AspectTupleVector<AspectAngle> vAspects;
  PlanetPairs pairs;
  std::unordered_map<Planet, std::vector<EssentialState>> eStates;
  MoonPhase phase;

  enum FixedStar { Algol, Regulus, Spica, Count };

  std::array<Longitude, Count> fixStars;
  double ut;
  double geolat;
  double geolon;

private:
};
}; // namespace specni