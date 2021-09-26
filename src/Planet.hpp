#pragma once

#include "swephpp.hpp"

namespace specni {
struct Planet {
  swephpp::PlanetaryBody Id;
  swephpp::PlanetEphData Data;
};
}; // namespace specni
