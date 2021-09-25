#pragma once

#include "ChartModel.hpp"

namespace specni {

class ControlsWidget {
public:
  ControlsWidget(ChartModel *model) : model(model) {}
  void Show() const;

private:
  ChartModel *model;
};
} // namespace specni