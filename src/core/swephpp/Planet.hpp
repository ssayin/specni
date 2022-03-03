#pragma once

#include <functional>

#include "Context.hpp"
#include "Cyclic.hpp"
#include "Defs.hpp"
#include "Ut.hpp"

namespace specni {
namespace core {
namespace swe {

struct PlanetEphData {
  double lon;
  double lat;
  double dist;
  double spdlon;
  double spdlat;
  double spddist;
};

class Planet;
std::ostream &operator<<(std::ostream &out, Planet const &p);

class Planet : ContextInit {
  friend std::ostream &operator<<(std::ostream &out, Planet const &p);

public:
  Planet(const Ipl &ipl, const Ut &ut, const EphFlag &flag);
  Planet(Planet &&) = default;

  auto Id() const -> const Ipl { return ipl; };
  auto Lon() const -> const EclipticLongitude { return x.lon; }
  auto Lat() const -> const double { return x.lat; }
  auto SpdLon() const -> const EclipticLongitude { return x.spdlon; }
  auto SpdLat() const -> const double { return x.spdlat; }
  auto FutureLon() const -> const EclipticLongitude {
    return {x.lon + x.spdlon};
  }
  auto Name() const -> const std::string;

  auto operator==(const Planet &o) const -> bool {
    return (o.ipl == this->ipl);
  }

private:
  Ipl ipl;
  PlanetEphData x{0};
};
}; // namespace swe
}; // namespace core
}; // namespace specni

namespace std {
template <> struct hash<specni::core::swe::Planet> {
  auto operator()(const specni::core::swe::Planet &p) const -> std::size_t {
    return std::hash<specni::core::swe::Ipl>()(p.Id());
  }
};
} // namespace std
