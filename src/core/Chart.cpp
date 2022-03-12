#include "Chart.hpp"
#include "Aspect.hpp"

namespace specni::core {

void printAspect(const swe::Planet& p1,
		const swe::Planet& p2,
		const std::optional<aspect::RetType>& asp)
{
	std::cout << p1.name() << " " << p2.name() << " "
			  << static_cast<int>(asp->type) << " "
			  << ((asp->detail==aspect::Detail::Applying) ? 'A' : 'S')
			  << " " << asp->orb << std::endl;
}

auto Combination(std::size_t N)
{
	std::string bitmask(2, 1);
	bitmask.resize(N, 0);

	std::vector<std::pair<std::size_t, std::size_t>> r;
	do {
		std::vector<size_t> accum;
		for (std::size_t i = 0u; i<N; ++i) {
			if (bitmask[i])
				accum.push_back(i);
		}
		r.emplace_back(accum[0], accum[1]);

	}
	while (std::prev_permutation(bitmask.begin(), bitmask.end()));

	return r;
}

Chart::Chart(const swe::Ut& ut, const swe::Coordinate& geodetic,
		swe::HouseSystem hs, const std::vector<swe::Ipl>& vIpl)
		:houses(ut, geodetic, hs), planets(ut, vIpl)
{


	auto planetPairs = Combination(planets.v.size());

	for (auto pair : planetPairs) {
		const swe::Planet& p1 = planets.m.at(planets.v[pair.first]);
		const swe::Planet& p2 = planets.m.at(planets.v[pair.second]);

		auto asp = aspect::HarmonicBetween<aspect::DefaultOrbConfig>(
				p1,
				p2);

		if (asp.has_value()) {
			printAspect(p1, p2, asp);
		}

		auto asp2 = aspect::DeclineBetween<aspect::OrbPartileConfig>(
				p1,
				p2);

		if (asp2.has_value()) {
			printAspect(p1, p2, asp2);
		}
	}
}

} // namespace specni
