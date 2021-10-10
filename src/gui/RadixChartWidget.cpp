#include "RadixChartWidget.hpp"
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

  ImVec2 outertext = ImVec2(0, window_size.y * settings.CuspTextRatio);
  ImVec2 outer = ImVec2(0, window_size.y * settings.CircleOuterRatio);
  ImVec2 inner = ImVec2(0, window_size.y * settings.CircleInnerRatio);
  ImVec2 innermost = ImVec2(0, window_size.y * settings.CircleInnermostRatio);
  ImVec2 housenumber = ImVec2(0, window_size.y * settings.CircleHouseNumbers);

  ImVec2 ascmcmid = (outertext + outer) / 2;

  // Draw circles
  draw_list->AddCircle(window_center, outer.y, settings.BaseColor, 0,
                       settings.Thickness);

  draw_list->AddCircle(window_center, inner.y, settings.BaseColor, 0,
                       settings.Thickness);

  draw_list->AddCircle(window_center, innermost.y, settings.BaseColor, 0,
                       settings.Thickness);

  draw_list->AddCircle(window_center, housenumber.y, settings.BaseColor, 0,
                       settings.Thickness);

  ImGui::PushFont(settings.font);

  // Draw sign glyphs
  char f[3];
  for (int i = 0; i < 12; i++) {
    float cos_a = cosf(util::DegToRad(-i * 30));
    float sin_a = sinf(util::DegToRad(-i * 30));

    float cos_b = cosf(util::DegToRad(-i * 30 + 15));
    float sin_b = sinf(util::DegToRad(-i * 30 + 15));
    draw_list->AddLine(window_center + ImRotate(inner, cos_a, sin_a),
                       window_center + ImRotate(outer, cos_a, sin_a),
                       settings.BaseColor, settings.Thickness);

    ImVec2 sign = (inner + outer) / 2;

    sprintf(f, "%c", 'a' + i);
    draw_list->AddText(window_center + ImRotate(sign, cos_b, sin_b),
                       settings.SignColor, f);
  }

  // Draw planet glyphs
  ImVec2 mid2 = ImVec2(0, window_size.y * 0.32f);
  for (std::vector<swephpp::PlanetEphData>::size_type i = 0;
       i < model->vEph.size(); ++i) {
    auto find = PlanetCharMap.find(model->vEph.at(i).Id);
    if (find != PlanetCharMap.end()) {
      sprintf(f, "%c", find->second);
      float cos_p = cosf(-util::DegToRad(model->vEph.at(i).Data.lon));
      float sin_p = sinf(-util::DegToRad(model->vEph.at(i).Data.lon));
      draw_list->AddText(window_center + ImRotate(mid2, cos_p, sin_p),
                         settings.PlanetColor, f);
    }
  }

  float cosac = cosf(-util::DegToRad(model->ascmc.ac));
  float sinac = sinf(-util::DegToRad(model->ascmc.ac));

  draw_list->AddLine(window_center + ImRotate(outer, cosac, sinac),
                     window_center + ImRotate(innermost, cosac, sinac),
                     settings.AscMcColor, settings.Thickness);

  cosac = cosf(-util::DegToRad(model->ascmc.mc));
  sinac = sinf(-util::DegToRad(model->ascmc.mc));

  draw_list->AddLine(window_center + ImRotate(outer, cosac, sinac),
                     window_center + ImRotate(innermost, cosac, sinac),
                     settings.AscMcColor, settings.Thickness);

  sprintf(f, "%c", 'K');
  draw_list->AddText(window_center + ImRotate(ascmcmid, cosac, sinac),
                     settings.AscMcColor, f);

  sprintf(f, "%c", 'L');

  draw_list->AddText(window_center + ImRotate(ascmcmid, cosac, sinac),
                     settings.AscMcColor, f);

  std::vector<ImVec2> vMidpoints;

  // Draw house cusps
  for (std::vector<float>::size_type i = 0; i < model->vHouseCusps.size();
       ++i) {

    float cosxd = cosf(-util::DegToRad(model->vHouseCusps.at(i)));
    float sinxd = sinf(-util::DegToRad(model->vHouseCusps.at(i)));

    ImVec2 EdgeA = window_center + ImRotate(innermost, cosxd, sinxd);

    ImVec2 Mid =
        (EdgeA + (window_center + ImRotate(housenumber, cosxd, sinxd))) / 2.0;

    vMidpoints.push_back(Mid);

    draw_list->AddLine(EdgeA, window_center + ImRotate(inner, cosxd, sinxd),
                       settings.BaseColor, settings.Thickness);
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