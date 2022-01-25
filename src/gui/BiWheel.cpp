
#include "BiWheel.hpp"

#include "ImVecEx.hpp"
#include "gui/UniWheel.hpp"
#include "imgui.h"
#include "util/ZodiacsFont.hpp"

namespace specni {
void BiWheel::Show() const {
  innerChart.BeginDraw();
  innerChart.DrawChart();

  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  // const ImVec2 p = ImGui::GetCursorScreenPos();
  ImVec2 window_pos = ImGui::GetWindowPos();
  ImVec2 window_size = ImGui::GetWindowSize();
  ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f,
                                window_pos.y + window_size.y * 0.5f);

  std::array<ImVec2, UniWheel::ChartSettings::Count> RPoints;

  for (int i = 0; i < UniWheel::ChartSettings::Count; ++i) {
    RPoints[i] = ImVec2(0, window_size.y * innerChart.settings.Ratios[i]);
  }

  ImVec2 RAscMc = (RPoints[UniWheel::ChartSettings::CuspText] +
                   RPoints[UniWheel::ChartSettings::SignOuter]) /
                  2;
  ImVec2 RHouseNumber = (RPoints[UniWheel::ChartSettings::Innermost] +
                         RPoints[UniWheel::ChartSettings::CircleHouseNumbers]) /
                        2;
  ImVec2 RSign = (RPoints[UniWheel::ChartSettings::SignInner] +
                  RPoints[UniWheel::ChartSettings::SignOuter]) /
                 2;

  float CosA = 0;
  float SinA = 0;

  char f[3];

  ImGui::PushFont(innerChart.settings.font);

  // Draw planet glyphs
  ImVec2 mid2 = ImVec2(0, window_size.y * 0.32f);
  for (auto &p : outerModel->Eph) {
    auto find = PlanetCharMap.find(p.first);
    if (find != PlanetCharMap.end()) {
      sprintf(f, "%c", find->second.first);
      std::tie(CosA, SinA) =
          innerChart.GetDegreeCosSinRotAsc(p.second.Data.lon);
      draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA),
                         ImColor(ImVec4(0.0, 1.0, 0.0, 1.0f)), f);
    }
  }

  ImGui::PopFont();

  innerChart.EndDraw();
}

}; // namespace specni
