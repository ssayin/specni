#pragma once

#include "ChartModel.hpp"

namespace specni {

class PlanetsWidget {
public:
  PlanetsWidget(specni::ChartModel *model) : model(model) {}
  void Show() const;

private:
  specni::ChartModel *model;
};
}; // namespace specni