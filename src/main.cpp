#include "house.h"
#include "planetinfo.h"
#include "swe/swephexp.h"

#include <iostream>
#include <tuple>

int main(int argc, char **argv) {
  double ut = swe_julday(2020, 11, 7, 10, 1);

  planet_info *pi = new planet_info();
  pi->set_ut(ut);
  for (int i = 0; i < SE_TRUE_NODE + 1; ++i) {
    pi->set_planetid(i);
    pi->calc();
    auto data = pi->get_sign();
    std::cout << pi->get_planet_name() << " " << std::get<0>(data) << " "
              << std::get<1>(data) << "\n";
  }

  std::cout << "House system: " << swephpp::house_name(swephpp::Alcabitius)
            << "\n";

  return 0;
}
