#include "Planet.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

#include <iostream>
#include <tuple>

namespace specni::core::swe {

std::ostream& operator<<(std::ostream& out, Planet const& p)
{
	out << p.name() << " " << p.longitude() << " " << p.latitude();
	return out;
}

// FIXME: Error check required
Planet::Planet(const Ipl& ipl, const Ut& ut, const EphFlag& flag)
		:ipl(ipl)
{
	ErrStr err;
	swe_calc_ut(ut, static_cast<std::underlying_type_t<Ipl>>(ipl),
			static_cast<std::underlying_type_t<EphFlag>>(flag),
			ecliptic.data(), err);

	swe_calc_ut(
			ut, static_cast<std::underlying_type_t<Ipl>>(ipl),
			static_cast<std::underlying_type_t<EphFlag>>(flag | EphFlag::Equatorial),
			equatorial.data(), err);
}

auto Planet::name() const -> std::string
{
	char ret[SE_MAX_STNAME];
	swe_get_planet_name(static_cast<std::underlying_type_t<Ipl>>(ipl), ret);
	return ret;
}

} // namespace specni
