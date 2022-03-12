#pragma once

#include "Ut.hpp"
#include "Defs.hpp"
#include "EphData.hpp"
#include "Base.hpp"

namespace specni::core::swe {
class FixedStar: ContextInit, public HasEcliptic {
public:
	FixedStar(std::string id, const Ut& ut, const EphFlag& flag);
	Longitude longitude() const override;
	double latitude() const override;

	std::string id;
	EphData ecliptic{0};

};

}
