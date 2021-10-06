#include "PlanetsWidget.hpp"

#include <imgui.h>
#include <include/Config.hpp>
#include <include/ZodiacsFont.hpp>
#include <util/util.hpp>

void specni::PlanetsWidget::Show() const {
  ImGui::Begin("Planets");
  if (ImGui::BeginTable("split2", 3, PlanetWidgetTableFlags)) {
    ImGui::TableSetupColumn("Object");
    ImGui::TableSetupColumn("Symbol");
    ImGui::TableSetupColumn("Sign/DMS");
    ImGui::TableHeadersRow();
    for (std::vector<swephpp::PlanetEphData>::size_type i = 0;
         i < model->vEph.size(); i++) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("%s", swephpp::planet_name(model->vEph.at(i).Id).c_str());
      ImGui::TableNextColumn();
      auto it = PlanetCharMap.find(model->vEph.at(i).Id);
      if (it != PlanetCharMap.end()) {
        ImGui::PushFont(this->font);
        ImGui::Text("%c", it->second);
        ImGui::PopFont();
      } else {
        ImGui::Text("%c", ' ');
      }
      ImGui::TableNextColumn();
      ImGui::Text(
          "%s", specni::util::get_sign_deg(model->vEph.at(i).Data.lon).c_str());
    }
    ImGui::EndTable();
  }

  ImGui::End();
}