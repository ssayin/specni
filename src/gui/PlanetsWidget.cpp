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

      ImGui::SameLine();
      ImGui::Text("%s", swephpp::planet_name(x.first).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%c", IsRetrograde(x.second) ? 'R' : '-');
      ImGui::TableNextColumn();
      ImGui::Text("%s", specni::util::get_sign_deg(x.second.Data.lon).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s%f%s", x.second.Data.spdlon >= 0 ? "+" : "",
                  x.second.Data.spdlon, IsSwift(x.second) ? " (Fast)" : "");
      ImGui::TableNextColumn();

      ImGui::Text(
          "%d", specni::GetHouseNum(x.second, model->vHouseCusps, model->hsys));
    }
  }

  ImGui::EndTable();

  ImGui::End();
}
