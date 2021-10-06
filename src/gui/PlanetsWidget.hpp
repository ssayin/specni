#pragma once

#include "ChartModel.hpp"

#include "imgui.h"

namespace specni {

class PlanetsWidget {
public:
  PlanetsWidget(specni::ChartModel *model, ImFont *font)
      : model(model), font(font) {}
  void Show() const;

private:
  specni::ChartModel *model;
  ImFont *font;
};
}; // namespace specni