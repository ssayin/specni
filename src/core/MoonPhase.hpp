#pragma once

namespace specni {

enum MoonPhase {
  NewMoon,
  WaxingCrescent,
  FirstQuarter,
  WaxingGibbous,
  FullMoon,
  WaningGibbous,
  LastQuarter,
  WaningCrescent
};

int DaysSinceNewMoon(int y, int m, double ut);

MoonPhase GetMoonPhase(int y, int m, double ut);
}; // namespace specni