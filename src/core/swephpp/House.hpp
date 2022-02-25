#pragma once

#include "Context.hpp"
#include "Defs.hpp"
#include "Ut.hpp"

#include <vector>

namespace specni {
namespace core {
namespace swe {

typedef struct {
  double ac;
  double mc;
  double armc;
  double vertex;
  double equasc; /* equatorial ascendant */
  double coasc1; /* co-ascendant (W. Koch) */
  double coasc2; /* co-ascendant (M. Munkasey)*/
  double polasc; /* polar ascendant (M. Munkasey)*/
  double res[2];
} Angles;

struct Coordinate {
  double lat;
  double lon;
};

class House {
public:
  House(const Ut &ut, ZodiacType ztype, const Coordinate &geodetic,
        HouseSystem hsys);

private:
  std::vector<double> cusps;
  Angles ang;
};

}; // namespace swe
}; // namespace core
}; // namespace specni
