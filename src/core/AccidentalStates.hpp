#pragma once

#include "core/ChartModel.hpp"
#include <algorithm>
#include <core/Cyclic.hpp>
#include <core/Planet.hpp>
#include <core/swephpp.hpp>
#include <cstdlib>
#include <unordered_map>
#include <vector>

namespace specni {

class AccidentalStates {
public:
  AccidentalStates(ChartModel &model) : model(model) {}
  int GetHouseNum(const Planet &p);

  bool IsSwift(const Planet &);
  bool IsSlow(const Planet &);

  inline bool IsRetrograde(const Planet &p) { return (p.Data.spdlon < 0.0); }
  inline bool IsDirect(const Planet &p) { return !IsRetrograde(p); }

  bool IsOriental(const Planet &);
  bool IsOccidental(const Planet &);
  bool IsUnderSunBeams(const Planet &);
  bool IsCombust(const Planet &);
  bool IsCazimi(const Planet &);

private:
  const ChartModel &model;
  const Planet &GetSun();
  bool IsWithinSun(const Planet &p, double deg);
};

} // namespace specni