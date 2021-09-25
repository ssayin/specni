#include "aspectswidget.hpp"
#include "swephpp.hpp"
#include <array>
#include <cmath>
#include <functional>
#include <imgui.h>
#include <sstream>
#include <unordered_map>

#include "util.hpp"

#include "cyclic.hpp"

namespace specni {

std::string AspectToString(Aspect asp) {
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
  ImGui::Begin("Aspects");

  std::stringstream ss;
  for (std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody, Aspect> &x :
       model->vAspects) {
    ss << swephpp::planet_name(std::get<0>(x)) << " "
       << swephpp::planet_name(std::get<1>(x)) << " "
       << AspectToString(std::get<2>(x));
    ss << std::endl;
  }

  ImGui::Text("%s", ss.str().c_str());

  ImGui::End();
}
}; // namespace specni
