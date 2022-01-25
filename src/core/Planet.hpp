#pragma once

#include "swephpp.hpp"
#include <functional>

namespace specni {
struct Planet {
  swephpp::Ipl Id;
  swephpp::PlanetEphData Data;

  auto operator==(const Planet &o) const -> bool { return (o.Id == this->Id); }
};
}; // namespace specni

namespace std {
template <> struct hash<specni::Planet> {
  auto operator()(const specni::Planet &p) const -> std::size_t {
    using std::hash;
    return std::hash<swephpp::Ipl>()(p.Id);
  }
};
} // namespace std