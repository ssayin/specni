#include "PlanetMap.hpp"

#include <utility>

namespace specni::core {

PlanetMap::PlanetMap(const swe::Ut& ut, std::vector<swe::Ipl> vIpl)
		:v(std::move(vIpl))
{
	constexpr
	swe::EphFlag flag = swe::EphFlag::SwissEph | swe::EphFlag::Speed;

	for (auto e : this->v) {
		m.insert({e, swe::Planet{e, ut, flag}});
	}
}

} // namespace specni
