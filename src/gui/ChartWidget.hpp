#pragma once

#include <imgui.h>
#include <include/swephpp.hpp>
#include <vector>

#include "ChartModel.hpp"
#include "ChartSettings.hpp"

namespace specni {

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