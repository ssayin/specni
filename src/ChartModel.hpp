#pragma once

#include "Aspects.hpp"
#include "swephpp.hpp"
#include <vector>

namespace specni {

struct ChartModel {

public:
  ChartModel() {
    SetDateGregorian(2021, 9, 23, 3.41);
    SetHouseSystem(swephpp::HouseSystem::Placidus);
    RecalculatePlanetPos();
    RecalculateHouses();
    RecalculateAspects();
  }

  void SetDateGregorian(int year, int month, int day, double hour) {
    this->ut = swe_julday(year, day, month, hour, 1);
  }

  void SetCoordinates(double latitude, double longitude) {
    geolat = latitude;
    geolon = longitude;
  }

  void SetHouseSystem(swephpp::HouseSystem sys) { hsys = sys; }

  void RecalculateAspects();

  void RecalculatePlanetPos();

  void RecalculateHouses();

  std::vector<swephpp::PlanetEphData> vEph;
  std::vector<float> vHouseCusps;
  AspectMatrix vAspects;
  swephpp::Angles ascmc;
  swephpp::HouseSystem hsys;

private:
  double ut;
  double geolat;
  double geolon;
};
}; // namespace specni