#include "Aspects.hpp"

namespace specni {

AspectMatrix
CalculateAspects(PlanetPairs &pairs,
                 std::function<Aspect(const Planet &, const Planet &)> f) {
  AspectMatrix ret;

  for (std::pair<Planet, Planet> &p : pairs) {
    Aspect asp = f(p.first, p.second);
    if (asp != Aspect::None) {
      ret.push_back({p.first.Id, p.second.Id, asp});
    }
  }

  return ret;
}
}; // namespace specni
