#pragma once

#include <algorithm>
#include <include/Planet.hpp>
#include <include/swephpp.hpp>
#include <vector>

namespace specni {
inline int GetHouseNum(const Planet &p, const std::vector<float> &houses,
                       swephpp::HouseSystem hsys) {

  return std::distance(
      houses.begin(),
      std::upper_bound(houses.begin(), houses.end(), p.Data.lon));
}

bool IsDirect(const Planet &);
bool IsRetrograde(const Planet &);
bool IsSwift(const Planet &);
bool IsSlow(const Planet &);
bool IsOriental(const Planet &);
bool IsOccidental(const Planet &);
bool IsUnderSunBeams(const Planet &);
bool IsCombust(const Planet &);
bool IsCazimi(const Planet &);

} // namespace specni