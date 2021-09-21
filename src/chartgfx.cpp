#include "chartgfx.hpp"
#include "imvecext.hpp"
#include "util.hpp"
#include <imgui.h>

static const char *GetHouseSystemString() {
  return "Equal\0Alcabitius\0Campanus\0EqualMC\0Carter\0Gauquelin\0Azimuth\0Sun"
         "shine\0Suns"
         "hineAlt\0Koch\0PullenSDelta\0Morinus\0WholeSign\0Porphyry\0Placidus\0"
         "PullenSRatio\0Regiomontanus\0Sripati\0PolichPage\0KrusinskiPisaGoelze"
         "r\0EqualVehlow\0EqualWholeSign\0ARSMeridian\0APC\0";
}

ChartGfx::ChartGfx(ChartSettings &settings) : settings(settings) {
  RecalculatePlanetPos();
  RecalculateHouses();
}

void ChartGfx::RecalculateHouses() {
  vHouseCusps.clear();
  swephpp::HouseOpts house_opts = {
      ut, swephpp::HouseCuspFlag::Tropical, geolat, geolon,
      static_cast<swephpp::HouseSystem>(
          (char)((houseSel > 3) ? (66 + houseSel) : (65 + houseSel)))};

  if (house_opts.hsys != swephpp::HouseSystem::Gauquelin) {
    swephpp::HouseCusps cusps;
    swephpp::houses_ex(house_opts, cusps, ascmc);
    for (int i = 1; i < cusps.max_size(); ++i)
      vHouseCusps.push_back(cusps[i]);

  } else {
    swephpp::GauquelinCusps cusps;
    swephpp::houses_ex(house_opts, cusps, ascmc);
    for (int i = 1; i < cusps.max_size(); ++i)
      vHouseCusps.push_back(cusps[i]);
  }
}

void ChartGfx::ChartWindow() {
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

  ImVec2 ascmcmid = (outertext + outer) / 2;

  draw_list->AddCircle(window_center, window_size.y * settings.CircleOuterRatio,
                       settings.BaseColor, 0, settings.Thickness);

  draw_list->AddCircle(window_center, window_size.y * settings.CircleInnerRatio,
                       settings.BaseColor, 0, settings.Thickness);

  draw_list->AddCircle(window_center,
                       window_size.y * settings.CircleInnermostRatio,
                       settings.BaseColor, 0, settings.Thickness);

  ImGui::PushFont(settings.font);

  char f[2];
  for (int i = 0; i < 12; i++) {
    float cos_a = cosf(DegToRad(i * 30));
    float sin_a = sinf(DegToRad(i * 30));

    float cos_b = cosf(DegToRad(i * 30 + 14));
    float sin_b = sinf(DegToRad(i * 30 + 14));
    draw_list->AddLine(window_center + ImRotate(inner, cos_a, sin_a),
                       window_center + ImRotate(outer, cos_a, sin_a),
                       settings.BaseColor, settings.Thickness);

    // ImVector<ImDrawVert> &buf = draw_list->VtxBuffer;
    // for (int i = 0; i < buf.Size; i++)
    // buf[i].pos = ImRotate(buf[i].pos, 1, 1) - (window_center +
    // ImRotate(mid, cos_b, sin_b)),

    ImVec2 sign = (inner + outer) / 2;

    sprintf(f, "%c", 'a' + i);
    draw_list->AddText(window_center + ImRotate(sign, cos_b, sin_b),
                       settings.SignColor, f);
  }

  ImVec2 mid2 = ImVec2(0, window_size.y * 0.32f);
  for (int i = 0; i < vEph.size(); ++i) {
    sprintf(f, "%c", 'A' + i);
    float cos_p = cosf(DegToRad(vEph.at(i).lon));
    float sin_p = sinf(DegToRad(vEph.at(i).lon));
    draw_list->AddText(window_center + ImRotate(mid2, cos_p, sin_p),
                       settings.PlanetColor, f);
  }

  float cosac = cosf(DegToRad(ascmc.ac));
  float sinac = sinf(DegToRad(ascmc.ac));

  float cosmc = cosf(DegToRad(ascmc.mc));
  float sinmc = sinf(DegToRad(ascmc.mc));

  draw_list->AddLine(window_center + ImRotate(outer, cosac, sinac),
                     window_center + ImRotate(innermost, cosac, sinac),
                     settings.AscMcColor, settings.Thickness);

  draw_list->AddLine(window_center + ImRotate(outer, cosmc, sinmc),
                     window_center + ImRotate(innermost, cosmc, sinmc),
                     settings.AscMcColor, settings.Thickness);

  sprintf(f, "%c", 'K');
  draw_list->AddText(window_center + ImRotate(ascmcmid, cosac, sinac),
                     settings.AscMcColor, f);

  sprintf(f, "%c", 'L');

  draw_list->AddText(window_center + ImRotate(ascmcmid, cosmc, sinmc),
                     settings.AscMcColor, f);

  ImGui::PopFont();

  for (int i = 0; i < vHouseCusps.size(); ++i) {
    float cosxd = cosf(DegToRad(vHouseCusps.at(i)));
    float sinxd = sinf(DegToRad(vHouseCusps.at(i)));

    draw_list->AddLine(window_center + ImRotate(innermost, cosxd, sinxd),
                       window_center + ImRotate(inner, cosxd, sinxd),
                       settings.BaseColor, settings.Thickness);
  }

  ImGui::End();
}

void ChartGfx::PlanetsWindow() {
  ImGui::Begin("Planets");
  if (ImGui::BeginTable("split2", 3,
                        ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_NoSavedSettings |
                            ImGuiTableFlags_Borders)) {
    for (int i = 0; i < vEph.size(); i++) {
      auto &d = specni::util::get_sign_deg(vEph.at(i).lon);
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text(
          "%s",
          swephpp::planet_name(static_cast<swephpp::PlanetaryBody>(i)).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s", std::get<0>(d).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%f", std::get<1>(d));
    }
    ImGui::EndTable();
  }

  ImGui::End();
}

void ChartGfx::RecalculatePlanetPos() {
  swephpp::PlanetEphData data = {0};
  vEph.clear();
  for (int i = 0; i < 9; ++i) {
    swephpp::CalcOpts opts = {
        ut,
        i,
        swephpp::Flag::SwissEph,
    };

    swephpp::calc(opts, data);
    vEph.push_back(data);
  }
}

void ChartGfx::ControlsWindow() {
  ImGui::Begin("Controls");

  bool changeFlag = false;

  ImGui::InputInt("Month", &m);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::InputInt("Day", &d);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::InputInt("Year", &y);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::InputDouble("Hour", &h);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::InputDouble("Latitude", &geolat);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::InputDouble("Longitude", &geolon);
  changeFlag |= ImGui::IsItemEdited();

  static int prev = 0;
  ImGui::Combo("HouseSystem", &houseSel, ::GetHouseSystemString());
  changeFlag |= (prev != houseSel);
  prev = houseSel;

  ImGui::End();
  if (changeFlag) {
    ut = swe_julday(y, m, d, h, 1);
    RecalculatePlanetPos();
    RecalculateHouses();
  }
}