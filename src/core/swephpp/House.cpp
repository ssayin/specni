#include "House.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

namespace specni::core::swe {

House::House(const Ut& ut, Coordinate geodetic, HouseSystem hsys)
		:geodetic(geodetic), hsys(hsys)
{
	constexpr
	std::size_t GauquelinSize = 37;
	constexpr
	std::size_t DefaultSize = 13;

	(hsys!=HouseSystem::Gauquelin) ? cusps.resize(DefaultSize)
			: cusps.resize(GauquelinSize);
	swe_houses(ut, geodetic.lat, geodetic.lon,
			static_cast<std::underlying_type_t<HouseSystem>>(hsys), &cusps[0],
			reinterpret_cast<double*>(&ang));

	double x[6];
	char err[256];
	swe_calc_ut(ut, SE_ECL_NUT,
			static_cast<std::underlying_type_t<EphFlag>>(EphFlag::SwissEph),
			x, err);
	nutationInObliquity = x[3];
}

double House::entityPos(const Longitude& lon, const Latitude& lat)
{
	char err[256];
	double pos[2] = {lon, lat};

	return swe_house_pos(ang.armc, geodetic.lat, nutationInObliquity,
			static_cast<std::underlying_type_t<HouseSystem>>(hsys),
			pos, err);
}

} // namespace specni
