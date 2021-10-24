#include <core/Aspects.hpp>
#include <core/ChartModel.hpp>
#include <core/Cyclic.hpp>
#include <core/PlanetPairs.hpp>
#include <core/PlanetStates.hpp>
#include <core/swephpp.hpp>

namespace specni {

void ChartModel::RecalculateAspects() {
  vAspects = CalculateAspects(pairs, AspectFunc<>);
}

void ChartModel::RecalculatePlanetPos() {
  swephpp::PlanetEphData data = {0};
  Eph.clear();
  for (int i = 0; i < 10; ++i) {
    swephpp::CalcOpts opts = {
        ut,
        i,
        swephpp::Flag::SwissEph | swephpp::Flag::Speed,
    };

    swephpp::calc(opts, data);
    Eph.insert({static_cast<swephpp::PlanetaryBody>(i),
                Planet{static_cast<swephpp::PlanetaryBody>(i), data}});
  }

  std::vector<Planet> vPlanet;
  for (std::unordered_map<swephpp::PlanetaryBody, Planet>::iterator it =
           Eph.begin();
       it != Eph.end(); ++it) {
    vPlanet.push_back(it->second);
  }

  pairs = GetPlanetCombPairs(vPlanet);
  eStates = PlanetStates(*this).GetPlanetEssentialStates();
  phase = GetMoonPhase(this->ut);
}

template <class CuspArray>
static std::vector<float> GetHouseCusps(const swephpp::HouseOpts &opts,
                                        swephpp::Angles &ascmc) {
  CuspArray tmpCusps;
  std::vector<float> cusps;
  swephpp::houses_ex(opts, tmpCusps, ascmc);
  for (typename CuspArray::size_type i = 1; i < tmpCusps.max_size(); ++i)
    cusps.push_back(tmpCusps[i]);

  return cusps;
}

void ChartModel::RecalculateHouses() {
  swephpp::HouseOpts house_opts = {ut, swephpp::HouseCuspFlag::Tropical, geolat,
                                   geolon, hsys};

  vHouseCusps = ((house_opts.hsys == swephpp::HouseSystem::Gauquelin)
                     ? GetHouseCusps<swephpp::GauquelinCusps>(house_opts, ascmc)
                     : GetHouseCusps<swephpp::HouseCusps>(house_opts, ascmc));
}
}; // namespace specni
