#include "AspectsWidget.hpp"
#include <array>
#include <cmath>
#include <functional>
#include <imgui.h>
#include <include/AccidentalStates.hpp>
#include <include/Aspects.hpp>
#include <sstream>
#include <unordered_map>

#include <util/util.hpp>

namespace specni {

void AspectsWidget::Show() const {

  std::stringstream ss;
  for (std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody, Aspect> &x :
       model->vAspects) {
    ss << swephpp::planet_name(std::get<0>(x)) << " "
       << swephpp::planet_name(std::get<1>(x)) << " "
       << util::AspectToString(std::get<2>(x));
    ss << std::endl;
  }

  ImGui::Begin("Aspects");

  ImGui::Text("%s", ss.str().c_str());

  ImGui::End();

  std::stringstream ss2;
  for (auto &p : model->vEph) {
    ss2 << swephpp::planet_name(p.Id) << " "
        << GetHouseNum(p, model->vHouseCusps, model->hsys) << std::endl;
  }

  ImGui::Begin("Houses");
  ImGui::Text("%s", ss2.str().c_str());
  ImGui::End();
}
}; // namespace specni
