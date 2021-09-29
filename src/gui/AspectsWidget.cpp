#include "AspectsWidget.hpp"
#include <array>
#include <cmath>
#include <functional>
#include <imgui.h>
#include <include/Aspects.hpp>
#include <sstream>
#include <unordered_map>

#include <util/util.hpp>

#include <include/cyclic.hpp>

namespace specni {

const std::string AspectToString(Aspect asp) {
  switch (asp) {
  case Conjunction:
    return "Conjunction";
  case Sextile:
    return "Sextile";
  case Square:
    return "Square";
  case Trine:
    return "Trine";
  case Opposition:
    return "Opposition";
  default:
    return "";
  }
}

void AspectsWidget::Show() const {

  std::stringstream ss;
  for (std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody, Aspect> &x :
       model->vAspects) {
    ss << swephpp::planet_name(std::get<0>(x)) << " "
       << swephpp::planet_name(std::get<1>(x)) << " "
       << AspectToString(std::get<2>(x));
    ss << std::endl;
  }

  ImGui::Begin("Aspects");

  ImGui::Text("%s", ss.str().c_str());

  ImGui::End();

  std::stringstream ss2;
  for (auto &p : model->vEph) {
    ss2 << swephpp::planet_name(p.Id) << " "
        << p.GetHouseNum(model->vHouseCusps, model->hsys) << std::endl;
  }

  ImGui::Begin("Houses");
  ImGui::Text("%s", ss2.str().c_str());
  ImGui::End();
}
}; // namespace specni
