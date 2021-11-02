#include "core/Dignities.hpp"
#include "core/Horary.hpp"
#include "core/MoonPhase.hpp"
#include "core/PlanetPairs.hpp"
#include "core/swephpp.hpp"
#include <array>
#include <cmath>
#include <core/Aspects.hpp>
#include <core/PlanetStates.hpp>
#include <functional>
#include <gui/AspectsWidget.hpp>
#include <imgui.h>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

#include <gui/Config.hpp>
#include <util/Util.hpp>

#include <core/AccidentalState.hpp>

namespace specni {

void AspectsWidget::Show() const {

  ImGui::Begin("Aspects");

  for (std::tuple<swephpp::Ipl, swephpp::Ipl, Aspect> &x : model->vAspects) {
    ImGui::Text("%s %s %s", swephpp::planet_name(std::get<0>(x)).c_str(),
                util::AspectToString(std::get<2>(x)).c_str(),
                swephpp::planet_name(std::get<1>(x)).c_str());
  }

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

  ImGui::End();

  ImGui::Begin("Scores");

  DignityScoreTable dts = GetDignityScoreTable(*model);

  for (auto &t : dts) {
    char sum = 0;

    for (auto &s : std::get<1>(t)) {
      sum += std::get<0>(s);
    }

    if (ImGui::CollapsingHeader(
            (swephpp::planet_name(std::get<0>(t)) + " " + std::to_string(sum))
                .c_str())) {
      for (auto &s : std::get<1>(t)) {
        ImGui::Text("%s %d", std::get<1>(s).c_str(), std::get<0>(s));
      }
    }
  }

  ImGui::End();
}

}; // namespace specni
