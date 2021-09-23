#pragma once

#include "chartmodel.hpp"

namespace specni {

class ControlsWidget {
public:
  ControlsWidget(ChartModel *model) : model(model) {}
  void Show();

private:
  ChartModel *model;
};
} // namespace specni