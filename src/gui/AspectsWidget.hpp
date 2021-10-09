#pragma once
#include <gui/ChartModel.hpp>
namespace specni {
class AspectsWidget {
public:
  AspectsWidget(ChartModel *model) : model(model) {}
  void Show() const;

private:
  ChartModel *model;
};
}; // namespace specni
