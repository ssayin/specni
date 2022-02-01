#include <array>
#include <core/Aspects.hpp>
#include <core/ChartModel.hpp>
#include <core/Cyclic.hpp>
#include <core/PlanetPairs.hpp>
#include <core/PlanetStates.hpp>
#include <core/swephpp.hpp>
#include <cstdio>

namespace specni {

void ChartModel::RecalculateAspects() {
  vAspects = CalculateAspects<AspectAngle, DefaultOrbConfig>(pairs);
}

void ChartModel::RecalculatePlanetPos() {
  swephpp::PlanetEphData data = {0};
  Eph.clear();
  for (int i = 0; i < 16; ++i) {
    if (i == 10 || i == 12 || i == 13 || i == 14)
      continue;
    swephpp::CalcOpts opts = {
        ut,
        i,
        swephpp::Flag::SwissEph | swephpp::Flag::Speed |
            swephpp::Flag::Equatorial,
    };

    swephpp::calc(opts, data);
    Eph.insert({static_cast<swephpp::Ipl>(i),
                Planet{static_cast<swephpp::Ipl>(i), data}});
  }

  std::vector<Planet> vPlanet;
  for (auto &it : Eph) {
    vPlanet.push_back(it.second);
  }

  pairs = GetPlanetCombPairs(vPlanet);
  eStates = PlanetStates(*this).GetPlanetEssentialStates();
  phase = GetMoonPhase(this->ut);
  GetStars();
}

template <class CuspArray>
auto GetHouseCusps(const swephpp::HouseOpts &opts, swephpp::Angles &ascmc)
    -> std::vector<float> {
  CuspArray tmpCusps;
  std::vector<float> cusps;
  swephpp::houses_ex(opts, tmpCusps, ascmc);
  for (typename CuspArray::size_type i = 1; i < tmpCusps.max_size(); ++i)
    cusps.emplace_back(tmpCusps[i]);

  return cusps;
}

void ChartModel::RecalculateHouses() {
  swephpp::HouseOpts house_opts = {ut, swephpp::HouseCuspFlag::Tropical, geolat,
                                   geolon, hsys};

  vHouseCusps = ((house_opts.hsys == swephpp::HouseSystem::Gauquelin)
                     ? GetHouseCusps<swephpp::GauquelinCusps>(house_opts, ascmc)
                     : GetHouseCusps<swephpp::HouseCusps>(house_opts, ascmc));
}

static auto fixstarname(ChartModel::FixedStar star) -> std::string {
  switch (star) {

  case ChartModel::Algol:
    return "Algol";
  case ChartModel::Regulus:
    return "Regulus";
  case ChartModel::Spica:
    return "Spica";
  default:
    return "";
  }

  return "";
}

void ChartModel::GetStars() {
  std::array<double, 6> tmp;
  std::array<char, 64> str;
  for (int i = 0; i < FixedStar::Count; ++i) {
    sprintf(str.data(), "%s", fixstarname(static_cast<FixedStar>(i)).c_str());

    swephpp::fixstar(this->ut, str.data(), swephpp::Flag::SwissEph, tmp);
    fixStars[i] = tmp[0];
  }
}
}; // namespace specni
