#pragma once

#include "imgui.h"

namespace specni {
struct ChartSettings {
  ImColor BaseColor = {};
  ImColor SignColor = {};
  ImColor PlanetColor = {};
  ImColor AscMcColor = {};

  const float CircleInnerRatio = 0.40f;
  const float CircleOuterRatio = 0.45f;
  const float CuspTextRatio = 0.50f;
  const float CircleInnermostRatio = 0.25f;
  const float CircleHouseNumbers = 0.28f;

  float Thickness = 1.0f;
  ImFont *font;

  ChartSettings(ImFont *font)
      : BaseColor(ImVec4(0.5f, 1.f, 0.5f, 1.f)),
        SignColor(ImVec4(1, 1, 0.5, 1)), PlanetColor(ImColor(25, 255, 255)),
        AscMcColor(ImVec4(0.5, 1, 1, 1)), font(font) {}
};
}; // namespace specni