#pragma once

#include <functional>

#include "Context.hpp"
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

class Planet {
public:
  Planet(const Ipl &ipl, const Ut &ut, const EphFlag &flag);
  Planet(Planet &&) = default;

  auto operator==(const Planet &o) const -> bool {
    return (o.ipl == this->ipl);
  }

public:
  Ipl ipl;
  PlanetEphData x;
};
}; // namespace swe
}; // namespace core
}; // namespace specni

namespace std {
template <> struct hash<specni::core::swe::Planet> {
  auto operator()(const specni::core::swe::Planet &p) const -> std::size_t {
    return std::hash<specni::core::swe::Ipl>()(p.ipl);
  }
};
} // namespace std
