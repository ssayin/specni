#include "house.h"
#include "planetinfo.h"
#include "swephexp.h"
#include "util.h"

#include <iostream>
#include <utility>

std::ostream &operator<<(std::ostream &out,
                         const std::pair<std::string, double> &p) {
  out << std::get<0>(p) << " " << std::get<1>(p);
  return out;
}

int main(int argc, char **argv) {
  double ut = swe_julday(2020, 11, 7, 10, 1);

  planet_info *pi = new planet_info();
  pi->set_ut(ut);
  for (int i = 0; i < SE_TRUE_NODE + 1; ++i) {
    pi->set_planetid(i);
    pi->calc();
    std::cout << swephpp::get_sign_deg(pi->get_longitude()) << "\n";
  }

  std::cout << "House system: " << swephpp::house_name(swephpp::Alcabitius)
            << "\n";

  swephpp::AscMc ascmc;
  swephpp::Houses cusps;
  swephpp::houses_ex(ut, swephpp::Tropical, 32.11, 31.22, swephpp::Equal, cusps,
                     ascmc);

  std::cout << "Ascendant " << swephpp::get_sign_deg(ascmc.ascendant) << "\n";

  return 0;
}
