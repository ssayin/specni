#pragma once

#include "imgui.h"
#include "swephpp.hpp"
#include <vector>

#include "ChartModel.hpp"
#include "ChartSettings.hpp"

namespace specni {

inline float DegToRad(float deg) { return (M_PI * deg) / 180; }

class ChartWidget {
public:
  ChartWidget(ChartSettings &settings, ChartModel *model);
  void Show() const;

private:
  ChartSettings settings;
  ChartModel *model;
  int houseSel = 0;
};
}; // namespace specni