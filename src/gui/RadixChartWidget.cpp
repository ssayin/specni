#include "RadixChartWidget.hpp"
#include "SDL_video.h"
#include <gui/ImVecEx.hpp>
#include <imgui.h>
#include <util/Util.hpp>
#include <util/ZodiacsFont.hpp>

namespace specni {

RadixChartWidget::RadixChartWidget(ChartSettings &settings, ChartModel *model)
    : settings(settings), model(model) {}

void RadixChartWidget::Show() const {
  struct Constraint {

    static void Square(ImGuiSizeCallbackData *data) {
      data->DesiredSize =
          ImVec2(std::max(data->DesiredSize.x, data->DesiredSize.y),
                 std::max(data->DesiredSize.x, data->DesiredSize.y));
    }
  };

  ImGui::SetNextWindowSizeConstraints(ImVec2(600, 600), ImVec2(800, 800),
                                      Constraint::Square);

  ImGui::Begin("Chart");
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  // const ImVec2 p = ImGui::GetCursorScreenPos();
  ImVec2 window_pos = ImGui::GetWindowPos();
  ImVec2 window_size = ImGui::GetWindowSize();
  ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f,
                                window_pos.y + window_size.y * 0.5f);

  std::array<ImVec2, ChartSettings::Count> RPoints;

  for (int i = 0; i < ChartSettings::Count; ++i) {
    RPoints[i] = ImVec2(0, window_size.y * settings.Ratios[i]);
  }

  ImVec2 RAscMc =
      (RPoints[ChartSettings::CuspText] + RPoints[ChartSettings::SignOuter]) /
      2;
  ImVec2 RHouseNumber = (RPoints[ChartSettings::Innermost] +
                         RPoints[ChartSettings::CircleHouseNumbers]) /
                        2;
  ImVec2 RSign =
      (RPoints[ChartSettings::SignInner] + RPoints[ChartSettings::SignOuter]) /
      2;

  // Draw circles
  draw_list->AddCircle(window_center, RPoints[ChartSettings::SignOuter].y,
                       settings.BaseColor, 0, settings.Thickness);

  draw_list->AddCircle(window_center, RPoints[ChartSettings::SignInner].y,
                       settings.BaseColor, 0, settings.Thickness);

  draw_list->AddCircle(window_center, RPoints[ChartSettings::Innermost].y,
                       settings.BaseColor, 0, settings.Thickness);

  draw_list->AddCircle(window_center,
                       RPoints[ChartSettings::CircleHouseNumbers].y,
                       settings.BaseColor, 0, settings.Thickness);

  ImGui::PushFont(settings.font);

  // Set ASC at -90 degrees
  float rotate = 90 + model->ascmc.ac;

  // Draw sign glyphs
  char f[3];
  const int SignCount = 12;
  for (int i = 0; i < SignCount; i++) {
    float cos_a = cosf(util::DegToRad(-i * 30 - rotate));
    float sin_a = sinf(util::DegToRad(-i * 30 - rotate));

    draw_list->AddLine(
        window_center +
            ImRotate(RPoints[ChartSettings::SignInner], cos_a, sin_a),
        window_center +
            ImRotate(RPoints[ChartSettings::SignOuter], cos_a, sin_a),
        settings.BaseColor, settings.Thickness);

    float cos_b = cosf(util::DegToRad(-i * 30 - 15 - rotate));
    float sin_b = sinf(util::DegToRad(-i * 30 - 15 - rotate));

    sprintf(f, "%c", 'a' + i);

    draw_list->AddText(window_center + ImRotate(RSign, cos_b, sin_b),
                       settings.SignColor, f);
  }

  // Draw planet glyphs
  ImVec2 mid2 = ImVec2(0, window_size.y * 0.32f);
  for (std::vector<swephpp::PlanetEphData>::size_type i = 0;
       i < model->vEph.size(); ++i) {
    auto find = PlanetCharMap.find(model->vEph.at(i).Id);
    if (find != PlanetCharMap.end()) {
      sprintf(f, "%c", find->second.first);
      float cos_p = cosf(-util::DegToRad(model->vEph.at(i).Data.lon - rotate));
      float sin_p = sinf(-util::DegToRad(model->vEph.at(i).Data.lon - rotate));
      draw_list->AddText(window_center + ImRotate(mid2, cos_p, sin_p),
                         ImColor(find->second.second), f);
    }
  }

  float cosac = cosf(-util::DegToRad(model->ascmc.ac - rotate));
  float sinac = sinf(-util::DegToRad(model->ascmc.ac - rotate));

  draw_list->AddLine(
      window_center + ImRotate(RPoints[ChartSettings::SignOuter], cosac, sinac),
      window_center + ImRotate(RPoints[ChartSettings::Innermost], cosac, sinac),
      settings.AscMcColor, settings.Thickness);

  sprintf(f, "%c", 'K');
  draw_list->AddText(window_center + ImRotate(RAscMc, cosac, sinac),
                     settings.AscMcColor, f);

  cosac = cosf(-util::DegToRad(model->ascmc.mc - rotate));
  sinac = sinf(-util::DegToRad(model->ascmc.mc - rotate));

  draw_list->AddLine(
      window_center + ImRotate(RPoints[ChartSettings::SignOuter], cosac, sinac),
      window_center + ImRotate(RPoints[ChartSettings::Innermost], cosac, sinac),
      settings.AscMcColor, settings.Thickness);

  sprintf(f, "%c", 'L');

  draw_list->AddText(window_center + ImRotate(RAscMc, cosac, sinac),
                     settings.AscMcColor, f);

  std::vector<ImVec2> vMidpoints;

  // Draw house cusps
  for (std::vector<float>::size_type i = 0; i < model->vHouseCusps.size();
       ++i) {

    float cosxd = cosf(-util::DegToRad(model->vHouseCusps.at(i) - rotate));
    float sinxd = sinf(-util::DegToRad(model->vHouseCusps.at(i) - rotate));

    draw_list->AddLine(
        window_center +
            ImRotate(RPoints[ChartSettings::Innermost], cosxd, sinxd),
        window_center +
            ImRotate(RPoints[ChartSettings::SignInner], cosxd, sinxd),
        settings.BaseColor, settings.Thickness);

    vMidpoints.push_back(window_center + ImRotate(RHouseNumber, cosxd, sinxd));
  }

  for (std::vector<ImVec2>::size_type i = 1; i < vMidpoints.size(); ++i) {
    ImVec2 houseNumberPos = (vMidpoints.at(i) + vMidpoints.at(i - 1)) / 2.0;
    sprintf(f, "%d", static_cast<int>(i));
    draw_list->AddText(houseNumberPos, settings.AscMcColor, f);
  }

  if (!vMidpoints.empty()) {
    ImVec2 houseNumberPos =
        (vMidpoints.at(vMidpoints.size() - 1) + vMidpoints.at(0)) / 2.0;
    sprintf(f, "%d", static_cast<int>(vMidpoints.size()));
    draw_list->AddText(houseNumberPos, settings.AscMcColor, f);
  }

  ImGui::PopFont();

  ImGui::End();
}

}; // namespace specni