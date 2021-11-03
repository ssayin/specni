#pragma once
#include <core/ChartModel.hpp>

namespace specni {
class AspectsWidget {
public:
  AspectsWidget(ChartModel *model, ImFont &font) : model(model), font(font) {}
  void Show() const;

private:
  ChartModel *model;
  ImFont &font;
};
}; // namespace specni
