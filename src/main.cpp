#include "core/ChartModel.hpp"
#include <core/swephpp.hpp>

#include <iostream>

using namespace specni::core::swe;

auto imgui_main() -> void;

auto main(int, char **) -> int {
  GregorianTime g{2022, 2, 25, 11, 33, 16};

  specni::core::ChartModel model{g, Coordinate{44.44, 44.44},
                                 HouseSystem::Equal};
  return 0;
}
