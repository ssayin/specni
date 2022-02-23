#include "GUI.hpp"

namespace specni {
namespace gui {
FontHandler::FontHandler(ImGuiIO &io) {
  io.Fonts->AddFontDefault();

  const std::string fontPath = "../misc/fonts/zodiac_s.ttf";
  const float FontSizes[] = {13.0f, 16.0f};

  ImFontConfig cfg;
  cfg.GlyphMinAdvanceX = FontSizes[Zodiac13];
  // First font
  fonts[Zodiac13] =
      io.Fonts->AddFontFromFileTTF(fontPath.c_str(), FontSizes[Zodiac13], &cfg);

  cfg = {};
  cfg.GlyphOffset.x = cfg.GlyphOffset.y = -(FontSizes[Zodiac16] / 2.0);
  // Second font
  fonts[Zodiac16] =
      io.Fonts->AddFontFromFileTTF(fontPath.c_str(), FontSizes[Zodiac16], &cfg);

  io.Fonts->Build();
}

static auto GetHouseSystemString() -> const char * {
  return "Equal\0Alcabitius\0Campanus\0EqualMC\0Carter\0Gauquelin\0Azimuth\0Sun"
         "shine\0Suns"
         "hineAlt\0Koch\0PullenSDelta\0Morinus\0WholeSign\0Porphyry\0Placidus\0"
         "PullenSRatio\0Regiomontanus\0Sripati\0PolichPage\0KrusinskiPisaGoelze"
         "r\0EqualVehlow\0EqualWholeSign\0ARSMeridian\0APC\0";
}

ControlsWidget::ControlsWidget(const std::string id, specni::ChartModel *model)
    : id(id), model(model) {
  tm = (util::get_array_from_tuple(util::time_now()));
  loc = {45.0, 45.0};
  houseSel = 0;
  UpdateModel();
}

void ControlsWidget::UpdateModel() {
  model->SetHouseSystem(static_cast<swephpp::HouseSystem>(
      (char)((houseSel > 3) ? (66 + houseSel) : (65 + houseSel))));
  // model->SetDateGregorian(tm[0], tm[1], tm[2], tm[3], tm[4], tm[5]);
  // model->SetCoordinates(loc[0], loc[1]);
  // model->RecalculatePlanetPos();
  // model->RecalculateHouses();
  // model->RecalculateAspects();
}

void ControlsWidget::Show() {
  ImGui::Begin(id.c_str());

  bool changeFlag = false;

  const unsigned One = 1;
  const unsigned Zero = 0;
  const unsigned maxHour = 23;
  const unsigned maxMonth = 12;
  const unsigned maxMinSec = 59;

  ImGui::PushItemWidth(-1);
  float Avail = ImGui::CalcItemWidth();
  float LabelW = ImGui::CalcTextSize("Month").x;
  ImGui::PopItemWidth();
  ImGui::PushItemWidth((Avail / 3) - LabelW -
                       ImGui::GetStyle().ItemInnerSpacing.x);
  ImGui::DragScalar("Year", ImGuiDataType_U16, &tm[0], 0.5, NULL, NULL);

  changeFlag |= ImGui::IsItemEdited();

  ImGui::SameLine();

  ImGui::DragScalar("Mon", ImGuiDataType_U8, &tm[1], 0.5, &Zero, &maxMonth);
  changeFlag |= ImGui::IsItemEdited();

  int dayMax = util::last_day_of_month(tm[0], tm[1]);
  if (tm[2] > dayMax) {
    tm[2] = dayMax;
  }

  ImGui::SameLine();

  ImGui::DragScalar("Day", ImGuiDataType_U8, &tm[2], 0.2, &One, &dayMax);

  ImGui::DragScalar("Hour", ImGuiDataType_U8, &tm[3], 0.2, &Zero, &maxHour);

  ImGui::SameLine();
  ImGui::DragScalar("Min", ImGuiDataType_U8, &tm[4], 0.2, &Zero, &maxMinSec);

  ImGui::SameLine();
  ImGui::DragScalar("Sec", ImGuiDataType_U8, &tm[5], 0.2, &Zero, &maxMinSec);

  changeFlag |= ImGui::IsItemEdited();

  ImGui::PopItemWidth();

  ImGui::PushItemWidth(-1);
  Avail = ImGui::CalcItemWidth();
  LabelW = ImGui::CalcTextSize("Longitude").x;
  ImGui::PopItemWidth();
  ImGui::PushItemWidth((Avail / 2) - LabelW -
                       ImGui::GetStyle().ItemInnerSpacing.x);

  ImGui::InputDouble("Latitude", &loc[0]);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::SameLine();

  ImGui::InputDouble("Longitude", &loc[1]);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::PopItemWidth();

  static int prev = 0;

  ImGui::Combo("House System", &houseSel, GetHouseSystemString());
  changeFlag |= (prev != houseSel);
  prev = houseSel;

  ImGui::End();
  if (changeFlag) {
    UpdateModel();
  }
}
}; // namespace gui
}; // namespace specni
