#pragma once

#include <core/ChartModel.hpp>

namespace specni {

class ControlsWidget {
public:
  ControlsWidget(const std::string id, ChartModel *model);
  void Show();

  void UpdateModel();

private:
  ChartModel *model;
  int month;
  int day;
  int year;
  int hour;
  int min;
  int sec;

  double latitude;
  double longitude;
  int houseSel;

  std::string id;
};
} // namespace specni