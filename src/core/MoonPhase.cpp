#include "MoonPhase.hpp"

#include <cmath>
#include <util/Util.hpp>

// clang-format off
#include "swephexp.h"
// clang-format on

namespace specni {
/*
 based on Ben Danglish's Trig2
 http://www.ben-daglish.net/moon.shtml
*/
int DaysSinceNewMoon(double ut) {
  int y, m;
  swe_jdut1_to_utc(ut, 0, &y, &m, 0, 0, 0, 0);
  double n = std::floor(12.37 * (y - 1900 + ((1.0 * m - 0.5) / 12.0)));
  double t = n / 1236.85;
  double t2 = t * t;
  double as = 359.2242 + 29.105356 * n;
  double am = 306.0253 + 385.816918 * n + 0.010730 * t2;
  double xtra = 0.75933 + 1.53058868 * n + ((1.178e-4) - (1.55e-7) * t) * t2 +
                (0.1734 - 3.93e-4 * t) * std::sin(util::DegToRad(as)) -
                0.4068 * std::sin(util::DegToRad(am));
  int i = (xtra > 0.0) ? std::floor(xtra) : std::ceil(xtra - 1.0);
  int jd = (2415020 + 28 * n) + i;
  return std::fmod((ut - jd + 30), 30);
}

MoonPhase GetMoonPhase(double ut) {
  int phase = DaysSinceNewMoon(ut);
  switch (phase) {
  case 0:
  case 1:
  case 2:
  case 3:
    return NewMoon;
  case 4:
  case 5:
  case 6:
  case 7:
    return WaxingCrescent;
  case 8:
  case 9:
  case 10:
    return FirstQuarter;
  case 11:
  case 12:
  case 13:
    return WaxingGibbous;
  case 14:
  case 15:
  case 16:
  case 17:
    return FullMoon;
  case 18:
  case 19:
  case 20:
  case 21:
    return WaningGibbous;
  case 22:
  case 23:
  case 24:
  case 25:
    return LastQuarter;
  case 26:
  case 27:
  case 28:
  case 29:
    return WaningCrescent;
  default:
    return NewMoon;
  }
}

}; // namespace specni