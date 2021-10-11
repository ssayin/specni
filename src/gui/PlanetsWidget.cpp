#include "PlanetsWidget.hpp"
#include <core/AccidentalStates.hpp>

#include <gui/Config.hpp>
#include <imgui.h>
#include <util/Util.hpp>
#include <util/ZodiacsFont.hpp>

void specni::PlanetsWidget::Show() const {
  ImGui::Begin("Planets");
  if (ImGui::BeginTable("split2", 5, PlanetWidgetTableFlags)) {
    ImGui::TableSetupColumn("Body");
    ImGui::TableSetupColumn("Ret");
    ImGui::TableSetupColumn("Location");
    ImGui::TableSetupColumn("Velocity");
    ImGui::TableSetupColumn("House");
    ImGui::TableHeadersRow();
    for (std::vector<swephpp::PlanetEphData>::size_type i = 0;
         i < model->vEph.size(); i++) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      auto it = PlanetCharMap.find(model->vEph.at(i).Id);
      if (it != PlanetCharMap.end()) {
        ImGui::PushFont(this->font);
        ImGui::TextColored(it->second.second, "%c", it->second.first);

        ImGui::PopFont();
      } else {
        ImGui::Text("%c", ' ');
      }

      ImGui::SameLine();
      ImGui::Text("%s", swephpp::planet_name(model->vEph.at(i).Id).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%c", IsRetrograde(model->vEph[i]) ? 'R' : '-');
      ImGui::TableNextColumn();
      ImGui::Text(
          "%s", specni::util::get_sign_deg(model->vEph.at(i).Data.lon).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s%f%s", model->vEph[i].Data.spdlon >= 0 ? "+" : "",
                  model->vEph[i].Data.spdlon,
                  IsSwift(model->vEph[i]) ? " (Fast)" : "");
      ImGui::TableNextColumn();

      ImGui::Text("%d", specni::GetHouseNum(model->vEph.at(i),
                                            model->vHouseCusps, model->hsys));
    }
    ImGui::EndTable();
  }

  ImGui::End();
}