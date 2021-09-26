#include "PlanetsWidget.hpp"

#include "imgui.h"
#include "util.hpp"

void specni::PlanetsWidget::Show() const {
  ImGui::Begin("Planets");
  if (ImGui::BeginTable(
          "split2", 2,
          ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
              ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter |
              ImGuiTableFlags_BordersV)) {
    ImGui::TableSetupColumn("Object");
    ImGui::TableSetupColumn("Sign/DMS");
    ImGui::TableHeadersRow();
    for (std::vector<swephpp::PlanetEphData>::size_type i = 0;
         i < model->vEph.size(); i++) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("%s", swephpp::planet_name(model->vEph.at(i).Id).c_str());
      ImGui::TableNextColumn();
      ImGui::Text(
          "%s", specni::util::get_sign_deg(model->vEph.at(i).Data.lon).c_str());
    }
    ImGui::EndTable();
  }

  ImGui::End();
}