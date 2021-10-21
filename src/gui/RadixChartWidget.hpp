#pragma once

#include <imgui.h>
#include <vector>

#include "ChartModel.hpp"

namespace specni {

class RadixChartWidget {
public:
  struct ChartSettings {
    ImColor BaseColor = {};
    ImColor SignColor = {};
    ImColor PlanetColor = {};
    ImColor AscMcColor = {};

    enum Ratio {
      Innermost,
      SignInner,
      SignOuter,
      CuspText,
      CircleHouseNumbers,
      Count
    };

    const std::array<float, Count> Ratios = {0.25f, 0.40f, 0.45f, 0.40f, 0.28f};

    float Thickness = 1.0f;
    ImFont *font;

    ChartSettings(ImFont *font)
        : BaseColor(ImVec4(0.5f, 1.f, 0.5f, 1.f)),
          SignColor(ImVec4(1, 1, 0.5, 1)), PlanetColor(ImColor(25, 255, 255)),
          AscMcColor(ImVec4(0.5, 1, 1, 1)), font(font) {}
  };

  RadixChartWidget(ChartSettings &settings, ChartModel *model);
  void Show() const;

private:
  const std::tuple<float, float> GetDegreeCosSinRotAsc(float deg) const;
  ChartSettings settings;
  ChartModel *model;
  int houseSel = 0;
};
}; // namespace specni