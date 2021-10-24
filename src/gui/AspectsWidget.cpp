#include "core/Dignities.hpp"
#include "core/swephpp.hpp"
#include <array>
#include <cmath>
#include <core/Aspects.hpp>
#include <core/PlanetStates.hpp>
#include <functional>
#include <gui/AspectsWidget.hpp>
#include <imgui.h>
#include <ostream>
#include <sstream>
#include <unordered_map>

#include <gui/Config.hpp>
#include <util/Util.hpp>

namespace specni {

void AspectsWidget::Show() const {

  std::stringstream ss;
  for (std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody, Aspect> &x :
       model->vAspects) {
    ss << swephpp::planet_name(std::get<0>(x)) << " "
       << util::AspectToString(std::get<2>(x)) << " "
       << swephpp::planet_name(std::get<1>(x));

    ss << std::endl;
  }

  ImGui::Begin("Aspects");

  ImGui::Text("%s", ss.str().c_str());

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

  for (auto &d : model->eStates) {
    if (ImGui::CollapsingHeader(swephpp::planet_name(d.first.Id).c_str()))
      for (auto &e : d.second) {
        ImGui::Text("%s", util::EssentialStateToString(e).c_str());
      }
  }

  ImGui::End();
}
}; // namespace specni
