#pragma once

#include "core/ChartModel.hpp"
#include "gui/UniWheel.hpp"

namespace specni {
class BiWheel {

public:
  BiWheel(const UniWheel::ChartSettings &settings, ChartModel *inner,
          ChartModel *outer)
      : innerChart(settings, inner), outerModel(outer) {}

  void Show() const;

private:
  UniWheel innerChart;
  ChartModel *outerModel;
};
}; // namespace specni