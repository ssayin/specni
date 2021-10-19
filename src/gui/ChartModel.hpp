#pragma once

#include <core/Aspects.hpp>
#include <core/EssentialStates.hpp>
#include <core/MoonPhase.hpp>
#include <core/Planet.hpp>
#include <core/PlanetPairs.hpp>
#include <core/swephpp.hpp>
#include <string>
#include <util/Util.hpp>
#include <vector>

namespace specni {

struct ChartModel {

public:
  ChartModel() = default;
  ~ChartModel() = default;

  void SetDateGregorian(int year, int month, int day, double hour) {
    this->ut = swe_julday(year, month, day, hour, 1);
  }

  void SetCoordinates(double latitude, double longitude) {
    geolat = latitude;
    geolon = longitude;
  }

  void SetHouseSystem(swephpp::HouseSystem sys) { hsys = sys; }

  void RecalculateAspects();

  void RecalculatePlanetPos();

  void RecalculateHouses();

  std::unordered_map<swephpp::PlanetaryBody, Planet> Eph;
  std::vector<float> vHouseCusps;
  AspectMatrix vAspects;
  swephpp::Angles ascmc;
  swephpp::HouseSystem hsys;
  PlanetPairs pairs;
  std::unordered_map<Planet, std::vector<EssentialState>> eStates;

private:
  double ut;
  double geolat;
  double geolon;
};
}; // namespace specni