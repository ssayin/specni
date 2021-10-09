#pragma once

#include <algorithm>
#include <core/Cyclic.hpp>
#include <core/Planet.hpp>
#include <core/swephpp.hpp>
#include <cstdlib>
#include <vector>

namespace specni {

int GetHouseNum(const Planet &p, const std::vector<float> &houses,
                swephpp::HouseSystem hsys);

bool IsSwift(const Planet &);
bool IsSlow(const Planet &);

inline bool IsRetrograde(const Planet &p) { return (p.Data.spdlon < 0.0); }
inline bool IsDirect(const Planet &p) { return !IsRetrograde(p); }

bool IsOriental(const Planet &);
bool IsOccidental(const Planet &);
bool IsUnderSunBeams(const Planet &);
bool IsCombust(const Planet &);
bool IsCazimi(const Planet &);

} // namespace specni