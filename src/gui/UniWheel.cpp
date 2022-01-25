#include "UniWheel.hpp"
#include "SDL_video.h"
#include <gui/ImVecEx.hpp>
#include <imgui.h>
#include <tuple>
#include <util/Util.hpp>
#include <util/ZodiacsFont.hpp>
#include <utility>

namespace specni {

constexpr auto GetDegreeCosSin(float deg) -> const std::tuple<float, float> {
  return std::make_tuple(
      cosf(util::DegToRad(-deg)),
      sinf(util::DegToRad(-deg))); // we want to draw counter-clockwise
}

auto
UniWheel::GetDegreeCosSinRotAsc(float deg) const -> const std::tuple<float, float> {
  Longitude lon = Longitude(deg) - Longitude(model->ascmc.ac) + Longitude(-90);
  return GetDegreeCosSin(lon());
}

void UniWheel::BeginDraw() const {
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
}

void UniWheel::EndDraw() const { ImGui::End(); }

UniWheel::UniWheel(ChartSettings settings, ChartModel *model)
    : settings(std::move(settings)), model(model) {}

void UniWheel::DrawChart() const {
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

  char f[3];

  const int SignCount = 12;
  const int SignSpanDegrees = 30;
  const int HalfSignSpanDegrees = SignSpanDegrees / 2;
  float CosA = 0;
  float SinA = 0;

  for (int i = 0; i < SignCount; i++) {
    std::tie(CosA, SinA) = GetDegreeCosSinRotAsc(i * SignSpanDegrees);

    draw_list->AddLine(
        window_center + ImRotate(RPoints[ChartSettings::SignInner], CosA, SinA),
        window_center + ImRotate(RPoints[ChartSettings::SignOuter], CosA, SinA),
        settings.BaseColor, settings.Thickness);

    sprintf(f, "%c", 'a' + i);

    std::tie(CosA, SinA) =
        GetDegreeCosSinRotAsc(i * SignSpanDegrees + HalfSignSpanDegrees);

    // Draw sign glyphs
    draw_list->AddText(window_center + ImRotate(RSign, CosA, SinA),
                       settings.SignColor, f);
  }

  // Draw planet glyphs
  ImVec2 mid2 = ImVec2(0, window_size.y * 0.32f);
  for (auto &p : model->Eph) {
    auto find = PlanetCharMap.find(p.first);
    if (find != PlanetCharMap.end()) {
      sprintf(f, "%c", find->second.first);
      std::tie(CosA, SinA) = GetDegreeCosSinRotAsc(p.second.Data.lon);
      draw_list->AddText(window_center + ImRotate(mid2, CosA, SinA),
                         ImColor(find->second.second), f);
    }
  }

  std::tie(CosA, SinA) = GetDegreeCosSinRotAsc(model->ascmc.ac);

  draw_list->AddLine(
      window_center + ImRotate(RPoints[ChartSettings::SignOuter], CosA, SinA),
      window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
      settings.AscMcColor, settings.Thickness);

  sprintf(f, "%c", 'K');
  draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA),
                     settings.AscMcColor, f);

  std::tie(CosA, SinA) = GetDegreeCosSinRotAsc(model->ascmc.mc);

  draw_list->AddLine(
      window_center + ImRotate(RPoints[ChartSettings::SignOuter], CosA, SinA),
      window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
      settings.AscMcColor, settings.Thickness);

  sprintf(f, "%c", 'L');

  draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA),
                     settings.AscMcColor, f);

  std::vector<ImVec2> vMidpoints;

  // Draw house cusps
  for (float vHouseCusp : model->vHouseCusps) {

    std::tie(CosA, SinA) = GetDegreeCosSinRotAsc(vHouseCusp);

    draw_list->AddLine(
        window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
        window_center + ImRotate(RPoints[ChartSettings::SignInner], CosA, SinA),
        settings.BaseColor, settings.Thickness);

    vMidpoints.push_back(window_center + ImRotate(RHouseNumber, CosA, SinA));
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

  for (std::tuple<swephpp::Ipl, swephpp::Ipl, Aspect, double, AspectStat> &x :
       model->vAspects) {
    float CosB;
    float SinB;

    std::tie(CosA, SinA) =
        GetDegreeCosSinRotAsc(model->Eph[std::get<0>(x)].Data.lon);
    std::tie(CosB, SinB) =
        GetDegreeCosSinRotAsc(model->Eph[std::get<1>(x)].Data.lon);

    draw_list->AddLine(
        window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
        window_center + ImRotate(RPoints[ChartSettings::Innermost], CosB, SinB),
        ImColor(aspectColor[static_cast<size_t>(std::get<2>(x))]));
  }
}

void UniWheel::Show() const {
  BeginDraw();
  DrawChart();
  EndDraw();
}

}; // namespace specni