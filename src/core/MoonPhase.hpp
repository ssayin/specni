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

auto DaysSinceNewMoon(double ut) -> int;

auto GetMoonPhase(double ut) -> MoonPhase;
}; // namespace specni