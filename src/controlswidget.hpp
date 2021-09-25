#pragma once

#include "chartmodel.hpp"

namespace specni {

class ControlsWidget {
public:
  ControlsWidget(ChartModel *model) : model(model) {}
  void Show() const;

private:
  ChartModel *model;
};
} // namespace specni