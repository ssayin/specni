#pragma once

#include "Context.hpp"
#include "Cyclic.hpp"
#include "Defs.hpp"
#include "Ut.hpp"

#include <vector>

namespace specni::core::swe {

typedef struct {
	double ac;
	double mc;
	double armc;
	double vertex;
	double equasc; /* equatorial ascendant */
	double coasc1; /* co-ascendant (W. Koch) */
	double coasc2; /* co-ascendant (M. Munkasey)*/
	double polasc; /* polar ascendant (M. Munkasey)*/
	double res1;
	double res2;
} Angles;

struct Coordinate {
	double lat;
	double lon;
};

class House: ContextInit {
public:
	House(const Ut& ut, Coordinate geodetic, HouseSystem hsys);

	double entityPos(const Longitude& lon, const Latitude& lat);
	Angles ang{0};
	std::vector<double> cusps;
private:

	Coordinate geodetic;
	HouseSystem hsys;

	double nutationInObliquity;
};

} // namespace specni
