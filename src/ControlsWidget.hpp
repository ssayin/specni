#pragma once

#include "ChartModel.hpp"

namespace specni {

class ControlsWidget {
public:
  ControlsWidget(ChartModel *model);
  void Show();

  void UpdateModel();

private:
  ChartModel *model;
  int month;
  int day;
  int year;
  double hour;
  double latitude;
  double longitude;
  int houseSel;
};
} // namespace specni