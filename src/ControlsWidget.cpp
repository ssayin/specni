#include "ControlsWidget.hpp"

#include "imgui.h"
#include "swephpp.hpp"

static const char *GetHouseSystemString() {
  return "Equal\0Alcabitius\0Campanus\0EqualMC\0Carter\0Gauquelin\0Azimuth\0Sun"
         "shine\0Suns"
         "hineAlt\0Koch\0PullenSDelta\0Morinus\0WholeSign\0Porphyry\0Placidus\0"
         "PullenSRatio\0Regiomontanus\0Sripati\0PolichPage\0KrusinskiPisaGoelze"
         "r\0EqualVehlow\0EqualWholeSign\0ARSMeridian\0APC\0";
}

void specni::ControlsWidget::Show() const {
  ImGui::Begin("Controls");

  bool changeFlag = false;

  static int month = 11;
  ImGui::InputInt("Month", &month);
  changeFlag |= ImGui::IsItemEdited();

  static int day = 22;
  ImGui::InputInt("Day", &day);
  changeFlag |= ImGui::IsItemEdited();

  static int year = 2021;
  ImGui::InputInt("Year", &year);
  changeFlag |= ImGui::IsItemEdited();

  static double hour = 13.12;
  ImGui::InputDouble("Hour", &hour);
  changeFlag |= ImGui::IsItemEdited();

  static double lat = 33.33;
  ImGui::InputDouble("Latitude", &lat);
  changeFlag |= ImGui::IsItemEdited();

  static double lon = 33.33;
  ImGui::InputDouble("Longitude", &lon);
  changeFlag |= ImGui::IsItemEdited();

  static int houseSel = 0;
  static int prev = 0;
  ImGui::Combo("HouseSystem", &houseSel, ::GetHouseSystemString());
  changeFlag |= (prev != houseSel);
  prev = houseSel;

  ImGui::End();
  if (changeFlag) {
    model->SetHouseSystem(static_cast<swephpp::HouseSystem>(
        (char)((houseSel > 3) ? (66 + houseSel) : (65 + houseSel))));
    model->SetDateGregorian(year, month, day, hour);
    model->SetCoordinates(lat, lon);
    model->RecalculatePlanetPos();
    model->RecalculateHouses();
    model->RecalculateAspects();
  }
}