#include "Horary.hpp"

namespace specni {
auto GetDignityScoreTable(const ChartModel &model) -> DignityScoreTable {
  /*
  constexpr static std::array<char, 13> houseScore{5, 3,  1, 4, 3, -2,
                                                   4, -2, 2, 5, 4, -5};

  constexpr static char suffix[][3]{"st", "nd", "rd", "th", "th", "th",
                                    "th", "th", "th", "th", "th", "th"};
  DignityScoreTable dts;

  PlanetStates ps(model);

  for (auto &d : model.eStates) {
    dts.insert({d.first.Id, std::vector<DignityTuple>()});
    for (auto &e : d.second) {
      int score = essentialScores[static_cast<size_t>(e)];
      dts[d.first.Id].push_back({score, util::EssentialStateToString(e)});
    }

    char houseNum = ps.GetHouseNum(d.first) - 1;
    char score = houseScore[houseNum];

    dts[d.first.Id].push_back(
        {score, std::string("In " + std::to_string(houseNum + 1) +
                            suffix[houseNum] + " house ")});

    if (d.first.Id != swephpp::Ipl::Sun && d.first.Id != swephpp::Ipl::Moon) {
      if (ps.IsDirect(d.first)) {
        dts[d.first.Id].push_back({4, "Direct in Motion"});
      } else {
        dts[d.first.Id].push_back({-5, "Retrograde"});
      }
    }

    if (d.first.Id == swephpp::Ipl::Mercury ||
        d.first.Id == swephpp::Ipl::Venus) {

      if (ps.IsOriental(d.first)) {
        dts[d.first.Id].push_back({-2, "Oriental"});
      } else {
        dts[d.first.Id].push_back({2, "Occidental"});
      }
    }

    if (d.first.Id == swephpp::Ipl::Saturn ||
        d.first.Id == swephpp::Ipl::Jupiter ||
        d.first.Id == swephpp::Ipl::Mars) {

      if (ps.IsOriental(d.first)) {
        dts[d.first.Id].push_back({2, "Oriental"});
      } else {
        dts[d.first.Id].push_back({-2, "Occidental"});
      }
    }

    if (d.first.Id != swephpp::Ipl::Sun) {
      if (ps.IsSwift(d.first)) {
        dts[d.first.Id].push_back({2, "Swift in Motion"});

      } else if (ps.IsSlow(d.first)) {
        dts[d.first.Id].push_back({-2, "Slow in Motion"});
      }
    }

    if (d.first.Id == swephpp::Ipl::Moon) {
      switch (model.phase) {
      case NewMoon:
      case WaxingCrescent:
      case FirstQuarter:
      case WaxingGibbous:
        dts[d.first.Id].push_back({2, "Increasing in Light"});
        break;

      case FullMoon:
        break;

      case WaningGibbous:
      case LastQuarter:
      case WaningCrescent:
        dts[d.first.Id].push_back({-2, "Decreasing in Light"});
        break;
      }
    }

    if (ps.IsCazimi(d.first)) {
      dts[d.first.Id].push_back({5, "Cazimi"});
    } else if (ps.IsCombust(d.first)) {
      dts[d.first.Id].push_back({-5, "Combust"});
    } else if (ps.IsUnderSunBeams(d.first)) {
      dts[d.first.Id].push_back({-4, "Under Sun's Beams"});
    } else {
      dts[d.first.Id].push_back({5, "Free from combustion and Sun's beams"});
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
        pairs.emplace_back(std::make_pair(d.first, asd));
    }

    AspectTuple<AspectAngle> aspects =
        CalculateAspects<AspectAngle>(pairs, AspectFunc<OrbPartileConfig>);

    for (auto &aspect : aspects) {
      swephpp::Ipl p2 = std::get<1>(aspect);
      AspectAngle asp = std::get<2>(aspect);

      switch (asp) {
      case AspectAngle::Conjunction:
        if (p2 == swephpp::Ipl::Venus) {
          dts[d.first.Id].push_back({5, "Partile conj. with Venus"});
        }
        if (p2 == swephpp::Ipl::Jupiter) {
          dts[d.first.Id].push_back({5, "Partile conj. with Jupiter"});
        }
        if (p2 == swephpp::Ipl::Saturn) {
          dts[d.first.Id].push_back({-5, "Partile conj. with Saturn"});
        }
        if (p2 == swephpp::Ipl::Mars) {
          dts[d.first.Id].push_back({-5, "Partile conj. with Mars"});
        }
        if (p2 == swephpp::Ipl::TrueNode) {
          dts[d.first.Id].push_back({4, "Partile conj. with NN"});
        }
        break;
      case AspectAngle::Sextile:
        if (p2 == swephpp::Ipl::Venus) {
          dts[d.first.Id].push_back({3, "Partile sextile with Venus"});
        }
        if (p2 == swephpp::Ipl::Jupiter) {
          dts[d.first.Id].push_back({3, "Partile sextile with Jupiter"});
        }
        break;
      case AspectAngle::Square:
        if (p2 == swephpp::Ipl::Saturn) {
          dts[d.first.Id].push_back({-4, "Partile square with Saturn"});
        }
        if (p2 == swephpp::Ipl::Mars) {
          dts[d.first.Id].push_back({-4, "Partile square with Mars"});
        }
        break;
      case AspectAngle::Trine:
        if (p2 == swephpp::Ipl::Venus) {
          dts[d.first.Id].push_back({4, "Partile trine with Venus"});
        }
        if (p2 == swephpp::Ipl::Jupiter) {
          dts[d.first.Id].push_back({4, "Partile trine with Jupiter"});
        }
        break;
      case AspectAngle::Opposition:
        if (p2 == swephpp::Ipl::Saturn) {
          dts[d.first.Id].push_back({-4, "Partile opp. with Saturn"});
        }
        if (p2 == swephpp::Ipl::Mars) {
          dts[d.first.Id].push_back({-4, "Partile opp. with Mars"});
        }
        if (p2 == swephpp::Ipl::TrueNode) {
          dts[d.first.Id].push_back({-4, "Partile conj. with SN"});
        }
        break;
      case AspectAngle::Count:
        break;
      }
    }

    Longitude pLon(d.first.Data.lon);
    const double fixedStarOrb = 6.0;
    if (pLon.within(model.fixStars[ChartModel::Algol], fixedStarOrb)) {
      dts[d.first.Id].push_back({-4, "Conj. with Algol"});
    }
    if (pLon.within(model.fixStars[ChartModel::Regulus], fixedStarOrb)) {
      dts[d.first.Id].push_back({6, "Conj. with Regulus"});
    }
    if (pLon.within(model.fixStars[ChartModel::Spica], fixedStarOrb)) {
      dts[d.first.Id].push_back({5, "Conj. with Spica"});
    }
  }
  return dts;
  */
}
}; // namespace specni
