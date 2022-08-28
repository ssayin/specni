#ifndef SPECNI_WHEEL_H
#define SPECNI_WHEEL_H

#include <array>
#include <imgui.h>

#include <Chart.hpp>
#include <swephpp.hpp>

namespace specni::gui {
static constexpr std::size_t ColorCount = 5;
using ColorPalette = std::array<ImColor, ColorCount>;

struct ChartSettings {
  ColorPalette colors{ImColor(0.5f, 0.5f, 0.5f), ImColor(0.2f, 0.2f, 0.5f)};
  enum Ratio {
    Innermost,
    SignInner,
    SignOuter,
    CuspText,
    CircleHouseNumbers,
    Count
  };

  static constexpr std::array<double, Count> CirclesRadii = {
      0.25f, 0.40f, 0.45f, 0.40f, 0.28f};

  static constexpr float Thickness = 1.0f;
  ImFont *font;

  explicit ChartSettings(ImFont *font) : font(font) {}
};

class Wheel {
public:
  Wheel(ChartSettings settings, core::Chart &model);
  void Show() const;

private:
  ChartSettings settings;
  core::Chart model;
};
} // namespace specni::gui

#endif // SPECNI_WHEEL_H
