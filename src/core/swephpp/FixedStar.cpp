#include <string>
#include "FixedStar.hpp"

#include "swephexp.h"

namespace specni {
namespace core {
namespace swe {

FixedStar::FixedStar(std::string id, const Ut& ut, const EphFlag& flag)
{
	char err[256];
	swe_fixstar_ut(id.data(), static_cast<double>(ut), static_cast<std::underlying_type_t<EphFlag>>(flag),
			ecliptic.data(), err);
	std::cout << err << std::endl;
}
Longitude FixedStar::longitude() const
{
	return Longitude{ecliptic[0]};
}

double FixedStar::latitude() const
{
	return ecliptic[1];
}
}
}
}