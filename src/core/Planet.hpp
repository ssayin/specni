#pragma once

#include "swephpp.hpp"
#include <functional>

namespace specni {
struct Planet {
  swephpp::PlanetaryBody Id;
  swephpp::PlanetEphData Data;

  bool operator==(const Planet &o) const { return (o.Id == this->Id); }
};
}; // namespace specni

namespace std {
template <> struct hash<specni::Planet> {
  std::size_t operator()(const specni::Planet &p) const {
    using std::hash;
    return std::hash<swephpp::PlanetaryBody>()(p.Id);
  }
};
} // namespace std