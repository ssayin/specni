#include "ChartModel.hpp"
#include <include/Aspects.hpp>
#include <include/cyclic.hpp>
#include <include/swephpp.hpp>

namespace specni {

void ChartModel::RecalculateAspects() {
  vAspects = CalculateAspects(vEph, AspectFunc<>);
}

void ChartModel::RecalculatePlanetPos() {
  swephpp::PlanetEphData data = {0};
  vEph.clear();
  for (int i = 0; i < 10; ++i) {
    swephpp::CalcOpts opts = {
        ut,
        i,
        swephpp::Flag::SwissEph,
    };

    swephpp::calc(opts, data);
    Planet p;
    p.Data = data;
    p.Id = static_cast<swephpp::PlanetaryBody>(i);
    vEph.push_back(p);
  }
}

void ChartModel::RecalculateHouses() {
  vHouseCusps.clear();
  swephpp::HouseOpts house_opts = {ut, swephpp::HouseCuspFlag::Tropical, geolat,
                                   geolon, hsys};

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

}; // namespace specni