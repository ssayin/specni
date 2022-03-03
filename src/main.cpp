#include "core/swephpp/Cyclic.hpp"
#include <core/Chart.hpp>
#include <core/swephpp.hpp>

using namespace specni::core;
using namespace specni::core::swe;

auto imgui_main() -> void;

auto main(int, char **) -> int {
  GregorianTime g{2022, 2, 25, 11, 33, 16};

  specni::core::Chart model{g,
                            Coordinate{44.44, 44.44},
                            HouseSystem::Equal,
                            {swe::Ipl::Sun, swe::Ipl::Moon, swe::Ipl::Mercury,
                             swe::Ipl::Venus, swe::Ipl::Mars,
                             swe::Ipl::Jupiter}};

  return 0;
}
