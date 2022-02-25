#pragma once

#include <gui/ImVecEx.hpp>
#include <imgui.h>
#include <memory>
#include <tuple>
#include <util/Calendar.hpp>
#include <utility>
#include <vector>

#include <core/ChartModel.hpp>
#include <gui/ChartSettings.hpp>

namespace specni {
namespace gui {

struct FontHandler {
  FontHandler(ImGuiIO &io);
  enum { Zodiac13, Zodiac16 };
  std::array<ImFont *, 2> fonts;

  inline void Push(int id) { ImGui::PushFont(fonts[id]); }

  inline void Pop() { ImGui::PopFont(); }
};

struct Resources {
  Resources(ImGuiIO &io) : fh(io) {}
  FontHandler fh;
  ChartModel model;
  ChartSettings settings;
};

class ControlsWidget {
public:
  ControlsWidget(const std::string id, ChartModel *model);
  void Show();

  void UpdateModel();

private:
  ChartModel *model;
  swephpp::Ut::GregorianTime tm;
  /* latitude, longitude */
  std::array<double, 2> loc;

  int houseSel;

  std::string id;
};

//  [[nodiscard]] auto GetDegreeCosSinRotAsc(float deg) const
// -> const std::tuple<float, float>;
/*
constexpr auto GetDegreeCosSin(float deg) -> const std::tuple<float, float> {
  return std::make_tuple(
    cosf(util::DegToRad(-deg)),
    sinf(util::DegToRad(-deg))); // we want to draw counter-clockwise
}

inline auto GetDegreeCosSinRotAsc(ChartModel* m, float deg)
-> const std::tuple<float, float> {
  Longitude lon = Longitude(deg) - Longitude(m->ascmc.ac) + Longitude(-90);
  return GetDegreeCosSin(lon());
}

inline void UniWheel::BeginDraw() const {
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

inline void UniWheel::EndDraw() const { ImGui::End(); }

UniWheel::UniWheel(ChartSettings settings, std::shared_ptr<ChartModel> model)
: settings(std::move(settings)), model(model) {}

void DrawChart() {
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

  for (const AspectTuple<AspectAngle> &x : model->vAspects) {
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

void specni::PlanetsWidget::Show() const {
  ImGui::Begin("Planets");
  if (ImGui::BeginTable("split2", 6, PlanetWidgetTableFlags)) {
    ImGui::TableSetupColumn("Body");
    ImGui::TableSetupColumn("Ret");
    ImGui::TableSetupColumn("Location");
    ImGui::TableSetupColumn("Velocity");
    ImGui::TableSetupColumn("House");
    ImGui::TableSetupColumn("Decl");
    ImGui::TableHeadersRow();
    for (auto &x : model->Eph) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      auto it = PlanetCharMap.find(x.first);
      if (it != PlanetCharMap.end()) {
        ImGui::PushFont(this->font);
        ImGui::TextColored(it->second.second, "%c", it->second.first);

        ImGui::PopFont();
      } else {
        ImGui::Text("%c", ' ');
      }

      PlanetStates ps(*model);

      ImGui::SameLine();
      ImGui::Text("%s", swephpp::planet_name(x.first).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%c", ps.IsRetrograde(x.second) ? 'R' : '-');
      ImGui::TableNextColumn();
      ImGui::Text("%s", specni::util::get_sign_deg(x.second.Data.lon).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s%f%s", x.second.Data.spdlon >= 0 ? "+" : "",
                  x.second.Data.spdlon, ps.IsSwift(x.second) ? " (Fast)" : "");
      ImGui::TableNextColumn();

      ImGui::Text("%d", ps.GetHouseNum(x.second));
      ImGui::TableNextColumn();

      int deg, min, sec;
      std::tie(deg, min, sec) = util::degtodms(x.second.Data.lat);
      ImGui::Text("%d %d'%d\"", deg, min, sec);
    }
  }

  ImGui::EndTable();

  ImGui::End();
}

namespace specni {
*/
/*
void AspectsWidget::Show() const {

  ImGui::Begin("Aspects");

  if (ImGui::BeginTable("aspects", 5, PlanetWidgetTableFlags)) {
    for (std::tuple<swephpp::Ipl, swephpp::Ipl, AspectAngle, double, AspectStat>
             &x : model->vAspects) {

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("%s ", swephpp::planet_name(std::get<0>(x)).c_str());

      ImGui::PushFont(font);
      ImGui::TableNextColumn();

      ImGui::Text("%c ", AspectCharMap.at(std::get<2>(x)));

      ImGui::PopFont();
      ImGui::TableNextColumn();

      ImGui::Text("%s ", swephpp::planet_name(std::get<1>(x)).c_str());

      ImGui::TableNextColumn();

      ImGui::Text("%f ", std::get<3>(x));

      ImGui::TableNextColumn();

      ImGui::Text("%c ", std::get<4>(x) == Applying ? 'A' : 'S');
    }
  }
  ImGui::EndTable();

  if (ImGui::BeginTable("aspects2", 5, PlanetWidgetTableFlags)) {
    for (std::tuple<swephpp::Ipl, swephpp::Ipl, Declination, double, AspectStat>
             &x :
         CalculateAspects<Declination, OrbPartileConfig>(model->pairs)) {

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("%s ", swephpp::planet_name(std::get<0>(x)).c_str());

      ImGui::TableNextColumn();

      ImGui::Text("%s ", std::get<2>(x) == Declination::Parallel
                             ? "Parallel"
                             : "Contra-parallel");

      ImGui::TableNextColumn();

      ImGui::Text("%s ", swephpp::planet_name(std::get<1>(x)).c_str());

      ImGui::TableNextColumn();

      ImGui::Text("%f ", std::get<3>(x));

      ImGui::TableNextColumn();

      ImGui::Text("%c ", std::get<4>(x) == Applying ? 'A' : 'S');
    }
  }
  ImGui::EndTable();
  ImGui::End();

  ImGui::Begin("Houses");

  if (ImGui::BeginTable("houses_table", 2, PlanetWidgetTableFlags)) {
    ImGui::TableSetupColumn("House");
    ImGui::TableSetupColumn("Sign/DMS");
    ImGui::TableHeadersRow();
    for (int i = 0; i < model->vHouseCusps.size(); ++i) {

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("House %d", (i + 1));
      ImGui::TableNextColumn();
      ImGui::Text("%s", util::get_sign_deg(model->vHouseCusps[i]).c_str());
    }

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Asc");
    ImGui::TableNextColumn();

    ImGui::Text("%s", util::get_sign_deg(model->ascmc.ac).c_str());

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Mc");
    ImGui::TableNextColumn();
    ImGui::Text("%s", util::get_sign_deg(model->ascmc.mc).c_str());

    ImGui::EndTable();
  }

  ImGui::End();

  ImGui::Begin("Scores");

  DignityScoreTable dts = GetDignityScoreTable(*model);

  for (auto &t : dts) {
    char sum = 0;

    for (auto &s : std::get<1>(t)) {
      sum += std::get<0>(s);
    }

    if (ImGui::CollapsingHeader(
            (swephpp::planet_name(std::get<0>(t)) + " " + std::to_string(sum))
                .c_str())) {
      for (auto &s : std::get<1>(t)) {
        ImGui::Text("%s %d", std::get<1>(s).c_str(), std::get<0>(s));
      }
    }
  }

  ImGui::End();
}
*/

}; // namespace gui
}; // namespace specni
