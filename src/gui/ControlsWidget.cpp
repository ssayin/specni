#include "ControlsWidget.hpp"
#include "util/Util.hpp"

#include <core/swephpp.hpp>
#include <imgui.h>
#include <util/Calendar.hpp>

static const char *GetHouseSystemString() {
  return "Equal\0Alcabitius\0Campanus\0EqualMC\0Carter\0Gauquelin\0Azimuth\0Sun"
         "shine\0Suns"
         "hineAlt\0Koch\0PullenSDelta\0Morinus\0WholeSign\0Porphyry\0Placidus\0"
         "PullenSRatio\0Regiomontanus\0Sripati\0PolichPage\0KrusinskiPisaGoelze"
         "r\0EqualVehlow\0EqualWholeSign\0ARSMeridian\0APC\0";
}

specni::ControlsWidget::ControlsWidget(specni::ChartModel *model)
    : model(model) {
  std::tie(year, month, day, hour, min, sec) = util::time_now();
  latitude = 45;
  longitude = 45;
  houseSel = 0;
  UpdateModel();
}

void specni::ControlsWidget::UpdateModel() {
  model->SetHouseSystem(static_cast<swephpp::HouseSystem>(
      (char)((houseSel > 3) ? (66 + houseSel) : (65 + houseSel))));
  model->SetDateGregorian(year, month, day, hour, min, sec);
  model->SetCoordinates(latitude, longitude);
  model->RecalculatePlanetPos();
  model->RecalculateHouses();
  model->RecalculateAspects();
}

void specni::ControlsWidget::Show() {
  ImGui::Begin("Controls");

  bool changeFlag = false;

  ImGui::PushItemWidth(-1);
  float Avail = ImGui::CalcItemWidth();
  float LabelW = ImGui::CalcTextSize("Month").x;
  ImGui::PopItemWidth();
  ImGui::PushItemWidth((Avail / 3) - LabelW -
                       ImGui::GetStyle().ItemInnerSpacing.x);
  ImGui::InputInt("Month", &month);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::SameLine();

  ImGui::InputInt("Day", &day);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::SameLine();

  ImGui::InputInt("Year", &year);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::InputInt("Hour", &hour);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::SameLine();

  ImGui::InputInt("Min", &min);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::SameLine();

  ImGui::InputInt("Sec", &sec);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::PopItemWidth();

  ImGui::PushItemWidth(-1);
  Avail = ImGui::CalcItemWidth();
  LabelW = ImGui::CalcTextSize("Longitude").x;
  ImGui::PopItemWidth();
  ImGui::PushItemWidth((Avail / 2) - LabelW -
                       ImGui::GetStyle().ItemInnerSpacing.x);

  ImGui::InputDouble("Latitude", &latitude);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::SameLine();

  ImGui::InputDouble("Longitude", &longitude);
  changeFlag |= ImGui::IsItemEdited();

  ImGui::PopItemWidth();

  static int prev = 0;

  ImGui::Combo("House System", &houseSel, ::GetHouseSystemString());
  changeFlag |= (prev != houseSel);
  prev = houseSel;

  ImGui::End();
  if (changeFlag) {
    UpdateModel();
  }
}