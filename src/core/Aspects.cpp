#include "Aspects.hpp"

namespace specni {

AspectTuple CalculateAspects(
    PlanetPairs &pairs,
    std::function<std::tuple<Aspect, double, AspectStat>(const Planet &,
                                                         const Planet &)>
        f) {
  AspectTuple ret;

  for (std::pair<Planet, Planet> &p : pairs) {
    auto asp = f(p.first, p.second);
    if (std::get<0>(asp) != Aspect::Count) {
      ret.push_back({p.first.Id, p.second.Id, std::get<0>(asp),
                     std::get<1>(asp), std::get<2>(asp)});
    }
  }

  return ret;
}
}; // namespace specni
