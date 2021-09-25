#include "PlanetsWidget.hpp"

#include "imgui.h"
#include "util.hpp"

void specni::PlanetsWidget::Show() const {
  ImGui::Begin("Planets");
  if (ImGui::BeginTable("split2", 3,
                        ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_NoSavedSettings |
                            ImGuiTableFlags_Borders)) {
    for (std::vector<swephpp::PlanetEphData>::size_type i = 0;
         i < model->vEph.size(); i++) {
      auto &d = specni::util::get_sign_deg(model->vEph.at(i).lon);
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