#include "core/Dignities.hpp"
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

constexpr static std::array<char, 13> houseScore{0, 5,  3, 1, 4, 3, -2,
                                                 4, -2, 2, 5, 4, -5};

constexpr static char suffix[][3]{" ",  "st", "nd", "rd", "th", "th", "th",
                                  "th", "th", "th", "th", "th", "th"};

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

  PlanetStates ps(*model);

  for (auto &d : model->eStates) {
    std::vector<std::tuple<char, std::string>> dignities;
    for (auto &e : d.second) {
      int score = essentialScores[static_cast<size_t>(e)];
      dignities.push_back({score, util::EssentialStateToString(e)});
    }

    char houseNum = ps.GetHouseNum(d.first);
    char score = houseScore[houseNum];

    dignities.push_back({score, std::string("In " + std::to_string(houseNum) +
                                            suffix[houseNum] + " house ")});

    if (d.first.Id != swephpp::Ipl::Sun && d.first.Id != swephpp::Ipl::Moon) {
      if (ps.IsDirect(d.first)) {
        dignities.push_back({4, "Direct in Motion"});
      } else {
        dignities.push_back({-5, "Retrograde"});
      }
    }

    if (d.first.Id == swephpp::Ipl::Mercury ||
        d.first.Id == swephpp::Ipl::Venus) {

      if (ps.IsOriental(d.first)) {
        dignities.push_back({-2, "Oriental"});
      } else {
        dignities.push_back({2, "Occidental"});
      }
    }

    if (d.first.Id == swephpp::Ipl::Saturn ||
        d.first.Id == swephpp::Ipl::Jupiter ||
        d.first.Id == swephpp::Ipl::Mars) {

      if (ps.IsOriental(d.first)) {
        dignities.push_back({2, "Oriental"});
      } else {
        dignities.push_back({-2, "Occidental"});
      }
    }

    if (d.first.Id != swephpp::Ipl::Sun) {
      if (ps.IsSwift(d.first)) {
        dignities.push_back({2, "Swift in Motion"});

      } else if (ps.IsSlow(d.first)) {
        dignities.push_back({-2, "Slow in Motion"});
      }
    }

    if (d.first.Id == swephpp::Ipl::Moon) {
      switch (model->phase) {
      case NewMoon:
      case WaxingCrescent:
      case FirstQuarter:
      case WaxingGibbous:
        dignities.push_back({2, "Increasing in Light"});
        break;

      case FullMoon:
        break;

      case WaningGibbous:
      case LastQuarter:
      case WaningCrescent:
        dignities.push_back({-2, "Decreasing in Light"});
        break;
      }
    }

    if (ps.IsCazimi(d.first)) {
      dignities.push_back({5, "Cazimi"});
    } else if (ps.IsCombust(d.first)) {
      dignities.push_back({-5, "Combust"});
    } else if (ps.IsUnderSunBeams(d.first)) {
      dignities.push_back({-4, "Under Sun's Beams"});
    } else {
      dignities.push_back({5, "Free from combustion and Sun's beams"});
    }

    std::vector<Planet> vSmall;
    vSmall.push_back(ps.GetPlanet(swephpp::Ipl::Venus));
    vSmall.push_back(ps.GetPlanet(swephpp::Ipl::Jupiter));
    vSmall.push_back(ps.GetPlanet(swephpp::Ipl::Mars));
    vSmall.push_back(ps.GetPlanet(swephpp::Ipl::Saturn));
    vSmall.push_back(ps.GetPlanet(swephpp::Ipl::TrueNode));

    PlanetPairs pairs;
    for (auto &asd : vSmall) {
      if (asd.Id != d.first.Id)
        pairs.push_back(std::make_pair(d.first, asd));
    }

    AspectTuple aspects = CalculateAspects(pairs, AspectFunc<OrbPartileConfig>);

    for (auto &aspect : aspects) {
      swephpp::Ipl p1;
      swephpp::Ipl p2;
      Aspect asp;
      std::tie(p1, p2, asp) = aspect;
      switch (asp) {
      case Conjunction:
        if (p2 == swephpp::Ipl::Venus) {
          dignities.push_back({5, "Partile conj. with Venus"});
        }
        if (p2 == swephpp::Ipl::Jupiter) {
          dignities.push_back({5, "Partile conj. with Jupiter"});
        }
        if (p2 == swephpp::Ipl::Saturn) {
          dignities.push_back({-5, "Partile conj. with Saturn"});
        }
        if (p2 == swephpp::Ipl::Mars) {
          dignities.push_back({-5, "Partile conj. with Mars"});
        }
        if (p2 == swephpp::Ipl::TrueNode) {
          dignities.push_back({4, "Partile conj. with NN"});
        }
        break;
      case Sextile:
        if (p2 == swephpp::Ipl::Venus) {
          dignities.push_back({3, "Partile sextile with Venus"});
        }
        if (p2 == swephpp::Ipl::Jupiter) {
          dignities.push_back({3, "Partile sextile with Jupiter"});
        }
        break;
      case Square:
        if (p2 == swephpp::Ipl::Saturn) {
          dignities.push_back({-4, "Partile square with Saturn"});
        }
        if (p2 == swephpp::Ipl::Mars) {
          dignities.push_back({-4, "Partile square with Mars"});
        }
        break;
      case Trine:
        if (p2 == swephpp::Ipl::Venus) {
          dignities.push_back({4, "Partile trine with Venus"});
        }
        if (p2 == swephpp::Ipl::Jupiter) {
          dignities.push_back({4, "Partile trine with Jupiter"});
        }
        break;
      case Opposition:
        if (p2 == swephpp::Ipl::Saturn) {
          dignities.push_back({-4, "Partile opp. with Saturn"});
        }
        if (p2 == swephpp::Ipl::Mars) {
          dignities.push_back({-4, "Partile opp. with Mars"});
        }
        if (p2 == swephpp::Ipl::TrueNode) {
          dignities.push_back({-4, "Partile conj. with SN"});
        }
        break;
      case None:
        break;
      }
    }

    Longitude pLon(d.first.Data.lon);
    const double fixedStarOrb = 6.0;
    if (pLon.within(model->fixStars[ChartModel::Algol], fixedStarOrb)) {
      dignities.push_back({-4, "Conj. with Algol"});
    }
    if (pLon.within(model->fixStars[ChartModel::Regulus], fixedStarOrb)) {
      dignities.push_back({6, "Conj. with Regulus"});
    }
    if (pLon.within(model->fixStars[ChartModel::Spica], fixedStarOrb)) {
      dignities.push_back({5, "Conj. with Spica"});
    }

    char sum = 0;
    for (auto &t : dignities) {
      sum += std::get<0>(t);
    }

    if (ImGui::CollapsingHeader(
            (swephpp::planet_name(d.first.Id) + " " + std::to_string(sum))
                .c_str())) {
      for (auto &t : dignities) {
        ImGui::Text("%s %d", std::get<1>(t).c_str(), std::get<0>(t));
      }
    }
  }

  ImGui::End();
}
}; // namespace specni
