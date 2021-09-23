#pragma once

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
  }

  void SetDateGregorian(int year, int month, int day, double hour) {
    this->ut = swe_julday(year, day, month, hour, 1);
  }

  void SetCoordinates(double latitude, double longitude) {
    geolat = latitude;
    geolon = longitude;
  }

  void SetHouseSystem(swephpp::HouseSystem sys) { hsys = sys; }

  std::vector<swephpp::PlanetEphData> vEph;
  std::vector<float> vHouseCusps;
  swephpp::Angles ascmc;
  swephpp::HouseSystem hsys;

  void RecalculatePlanetPos() {
    swephpp::PlanetEphData data = {0};
    vEph.clear();
    for (int i = 0; i < 9; ++i) {
      swephpp::CalcOpts opts = {
          ut,
          i,
          swephpp::Flag::SwissEph,
      };

      swephpp::calc(opts, data);
      vEph.push_back(data);
    }
  }

  void RecalculateHouses() {
    vHouseCusps.clear();
    swephpp::HouseOpts house_opts = {ut, swephpp::HouseCuspFlag::Tropical,
                                     geolat, geolon, hsys};

    if (house_opts.hsys != swephpp::HouseSystem::Gauquelin) {
      swephpp::HouseCusps cusps;
      swephpp::houses_ex(house_opts, cusps, ascmc);
      for (swephpp::HouseCusps::size_type i = 1; i < cusps.max_size(); ++i)
        vHouseCusps.push_back(cusps[i]);

    } else {
      swephpp::GauquelinCusps cusps;
      swephpp::houses_ex(house_opts, cusps, ascmc);
      for (swephpp::GauquelinCusps::size_type i = 1; i < cusps.max_size(); ++i)
        vHouseCusps.push_back(cusps[i]);
    }
  }

private:
  double ut;
  double geolat;
  double geolon;
};
}; // namespace specni