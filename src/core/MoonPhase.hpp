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

int DaysSinceNewMoon(double ut);

MoonPhase GetMoonPhase(double ut);
}; // namespace specni