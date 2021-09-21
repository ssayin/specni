#pragma once

#include "imgui.h"
#include "swephpp.hpp"
#include <vector>

inline float DegToRad(float deg) { return (M_PI * deg) / 180; }

class ChartSettings {
public:
  ImColor BaseColor = {};
  ImColor SignColor = {};
  ImColor PlanetColor = {};
  ImColor AscMcColor = {};

  const float CircleInnerRatio = 0.40f;
  const float CircleOuterRatio = 0.45f;
  const float CuspTextRatio = 0.50f;
  const float CircleInnermostRatio = 0.25f;

  float Thickness = 1.0f;
  ImFont *font;

  ChartSettings(ImFont *font)
      : font(font), BaseColor(ImVec4(0.5f, 1.f, 0.5f, 1.f)),
        SignColor(ImVec4(1, 1, 0.5, 1)), PlanetColor(ImColor(25, 255, 255)),
        AscMcColor(ImVec4(0.5, 1, 1, 1)) {}
};

class ChartGfx {
public:
  ChartGfx(ChartSettings &settings);
  void ChartWindow();
  void PlanetsWindow();
  void ControlsWindow();
  void RecalculatePlanetPos();
  void RecalculateHouses();

private:
  int m = 1;
  int d = 1;
  int y = 2020;
  double h = 11;
  double ut = swe_julday(y, d, m, h, 1);
  std::vector<swephpp::PlanetEphData> vEph;
  std::vector<float> vHouseCusps;
  swephpp::Angles ascmc;
  ChartSettings settings;
  double geolat = 33.33;
  double geolon = 33.33;
  int houseSel = 0;
};