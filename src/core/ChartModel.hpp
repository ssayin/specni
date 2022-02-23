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
#include <util/Calendar.hpp>
#include <util/Util.hpp>
#include <vector>

namespace specni {

struct ChartModel {
public:
  ChartModel(swephpp::Ut &&ut) : ut(std::move(ut)){};
  ChartModel() : ut(std::move(util::get_array_from_tuple(util::time_now()))){};

  // only change houses, UT is used
  void SetCoordinates(double latitude, double longitude) {
    geolat = latitude;
    geolon = longitude;
  }

  inline void RecalculateAspects() {
    vAspects = CalculateAspects<AspectAngle, DefaultOrbConfig>(pairs);
  }

  void SetHouseSystem(swephpp::HouseSystem sys) { hsys = sys; }

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
  double geolat;
  double geolon;

private:
  swephpp::Ut ut;
};
}; // namespace specni
