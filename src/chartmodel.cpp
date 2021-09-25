#include "chartmodel.hpp"
#include "aspects.hpp"
#include "cyclic.hpp"
#include "swephpp.hpp"

namespace specni {
struct OrbConfig {
  static constexpr int Get(swephpp::PlanetaryBody body) {
    switch (body) {
    case swephpp::Sun:
    case swephpp::Mercury:
    case swephpp::Moon:
    case swephpp::Venus:
    case swephpp::Mars:
    case swephpp::Jupiter:
    case swephpp::Saturn:
    case swephpp::Uranus:
    case swephpp::Neptune:
    case swephpp::Chiron:
    case swephpp::Pluto:
    case swephpp::MeanNode:
    case swephpp::TrueNode:
      return 5;
    }
  }
};

template <class Config = OrbConfig>
auto AspectFunc = [](const swephpp::PlanetEphData &eph1,
                     const swephpp::PlanetEphData &eph2) -> Aspect {
  auto a = longitude(eph1.lon);
  auto b = longitude(eph2.lon);
  for (auto &p : Aspects) {
    auto left = b + p.second;
    auto right = b - p.second;
    int maxOrb =
        std::max(Config::Get(swephpp::Mercury), Config::Get(swephpp::Mars));
    if (a.within(left, maxOrb) || a.within(right, maxOrb)) {
      return p.first;
    }
  }
  return Aspect::None;
};

void ChartModel::RecalculateAspects() {
  vAspects = CalculateAspects(vEph, AspectFunc<>);
}
}; // namespace specni