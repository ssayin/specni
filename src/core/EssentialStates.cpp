#include "EssentialStates.hpp"
namespace specni {

const std::unordered_map<Planet, std::vector<EssentialState>>
GetPlanetEssentialStates(std::vector<Planet> &planets) {
  std::unordered_map<Planet, std::vector<EssentialState>> ret;

  for (auto &p : planets) {
    ret.insert({p, std::vector<EssentialState>()});
    for (auto &ef : EssentialStateFunctions) {
      if (ef.second(p))
        ret.at(p).push_back(ef.first);
    }
    if (ret.empty())
      ret[p].push_back(EssentialState::Peregrine);
  }

  return ret;
}

} // namespace specni
