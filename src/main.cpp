#include "swephexp.h"
#include "util.h"

#include "swephpp.h"
#include "ptolemy.h"

#include <iostream>
#include <utility>

std::ostream &operator<<(std::ostream &out,
                         const std::pair<std::string, double> &p) {
  out << std::get<0>(p) << " " << std::get<1>(p);
  return out;
}

int main(int argc, char **argv) {
  double ut = swe_julday(2020, 11, 7, 10, 1);
  swephpp::PlanetEphData data = {0};

  for (int i = 0; i < 9; ++i) {
    swephpp::CalcOpts opts = {
        ut,
        i,
        swephpp::Flag::SwissEph,
    };

    swephpp::calc(opts, data);

    std::cout << swephpp::get_sign_deg(data.lon) << "\n";
  }

  swephpp::HousesOpts house_opts = {ut, swephpp::HouseCuspFlag::Tropical, 32.11,
                                    32.11, swephpp::HouseSystem::Equal};

  swephpp::HouseCusps cusps;
  swephpp::Angles ascmc;

  swephpp::houses_ex(house_opts, cusps, ascmc);

  for (int i = 1; i < 13; ++i) {
    std::cout << "House" << i << " " << swephpp::get_sign_deg(cusps[i]) << "\n";
  }

  std::cout << "Ascendant " << swephpp::get_sign_deg(ascmc.ac) << "\n";
  std::cout << "Midheaven " << swephpp::get_sign_deg(ascmc.mc) << "\n";


  double s;
  std::cin >> s;

  std::cout << swephpp::planetstr(specni::face(s));

  return 0;
}
