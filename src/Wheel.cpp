#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <imgui_internal.h>

#include <Chart.hpp>
#include <Combinations.hpp>
#include <FastMap.hpp>
#include <SwEphpp.hpp>
#include <ZFont.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <fmt/core.h>
#include <imgui.h>
#include <numbers>
#include <numeric>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <valarray>

namespace specni::gui {
static constexpr std::size_t ColorCount = 5;
using ColorPalette = std::array<ImColor, ColorCount>;
}; // namespace specni::gui

namespace specni::core {

// Aries - Mars, Taurus - Venus and so on.
constexpr std::array<swe::Ipl, 12> houseOrder /* domicile */ {
    swe::Ipl::Mars,    swe::Ipl::Venus,   swe::Ipl::Mercury, swe::Ipl::Moon,
    swe::Ipl::Sun,     swe::Ipl::Mercury, swe::Ipl::Venus,   swe::Ipl::Mars,
    swe::Ipl::Jupiter, swe::Ipl::Saturn,  swe::Ipl::Saturn,  swe::Ipl::Jupiter};

// The faces follow the pattern below
constexpr std::array<swe::Ipl, 7> faceOrder{
    swe::Ipl::Mars, swe::Ipl::Sun,    swe::Ipl::Venus,  swe::Ipl::Mercury,
    swe::Ipl::Moon, swe::Ipl::Saturn, swe::Ipl::Jupiter};

// Starting from the sign of Aries, Day / Night triplicity rulers
constexpr std::array<std::pair<swe::Ipl, swe::Ipl>, 12> triplicityRulers{
    std::make_pair(swe::Ipl::Sun, swe::Ipl::Jupiter),
    std::make_pair(swe::Ipl::Venus, swe::Ipl::Moon),
    std::make_pair(swe::Ipl::Saturn, swe::Ipl::Mercury),
    std::make_pair(swe::Ipl::Mars, swe::Ipl::Mars),
    std::make_pair(swe::Ipl::Sun, swe::Ipl::Jupiter),
    std::make_pair(swe::Ipl::Venus, swe::Ipl::Moon),
    std::make_pair(swe::Ipl::Saturn, swe::Ipl::Mercury),
    std::make_pair(swe::Ipl::Mars, swe::Ipl::Mars),
    std::make_pair(swe::Ipl::Sun, swe::Ipl::Jupiter),
    std::make_pair(swe::Ipl::Venus, swe::Ipl::Moon),
    std::make_pair(swe::Ipl::Saturn, swe::Ipl::Mercury),
    std::make_pair(swe::Ipl::Mars, swe::Ipl::Mars),
};

// Exact degree
constexpr auto exaltations = std::to_array<std::pair<swe::Ipl, double>>({
    {swe::Ipl::Moon, 33.0},     // Taurus 3
    {swe::Ipl::TrueNode, 63.0}, // Gemini 3
    {swe::Ipl::Jupiter, 105.0}, // Cancer 15
    {swe::Ipl::Mercury, 195.0}, // Virgo 15
    {swe::Ipl::Saturn, 231.0},  // Libra 21
    {swe::Ipl::Mars, 298.0},    // Aquarius 28
    {swe::Ipl::Venus, 357.0},   // Pisces 27
});

// Upper-bound
using TermsPentad = std::array<std::pair<swe::Ipl, double>, 5>;
constexpr auto terms = std::to_array<TermsPentad>({
    {
        std::make_pair(swe::Ipl::Jupiter, 6.0),
        std::make_pair(swe::Ipl::Venus, 14.0),
        std::make_pair(swe::Ipl::Mercury, 21.0),
        std::make_pair(swe::Ipl::Mars, 26.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Venus, 8.0),
        std::make_pair(swe::Ipl::Mercury, 15.0),
        std::make_pair(swe::Ipl::Jupiter, 22.0),
        std::make_pair(swe::Ipl::Saturn, 26.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mercury, 7.0),
        std::make_pair(swe::Ipl::Jupiter, 14.0),
        std::make_pair(swe::Ipl::Venus, 21.0),
        std::make_pair(swe::Ipl::Saturn, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mars, 6.0),
        std::make_pair(swe::Ipl::Jupiter, 13.0),
        std::make_pair(swe::Ipl::Mercury, 20.0),
        std::make_pair(swe::Ipl::Venus, 27.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Saturn, 6.0),
        std::make_pair(swe::Ipl::Mercury, 13.0),
        std::make_pair(swe::Ipl::Venus, 19.0),
        std::make_pair(swe::Ipl::Jupiter, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mercury, 7.0),
        std::make_pair(swe::Ipl::Venus, 13.0),
        std::make_pair(swe::Ipl::Jupiter, 18.0),
        std::make_pair(swe::Ipl::Saturn, 24.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Saturn, 7.0),
        std::make_pair(swe::Ipl::Venus, 13.0),
        std::make_pair(swe::Ipl::Jupiter, 18.0),
        std::make_pair(swe::Ipl::Mercury, 24.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mars, 6.0),
        std::make_pair(swe::Ipl::Jupiter, 14.0),
        std::make_pair(swe::Ipl::Venus, 21.0),
        std::make_pair(swe::Ipl::Mercury, 27.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Jupiter, 8.0),
        std::make_pair(swe::Ipl::Venus, 14.0),
        std::make_pair(swe::Ipl::Mercury, 19.0),
        std::make_pair(swe::Ipl::Saturn, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Venus, 6.0),
        std::make_pair(swe::Ipl::Mercury, 12.0),
        std::make_pair(swe::Ipl::Jupiter, 19.0),
        std::make_pair(swe::Ipl::Mars, 25.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Saturn, 6.0),
        std::make_pair(swe::Ipl::Mercury, 12.0),
        std::make_pair(swe::Ipl::Venus, 20.0),
        std::make_pair(swe::Ipl::Jupiter, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Venus, 8.0),
        std::make_pair(swe::Ipl::Jupiter, 14.0),
        std::make_pair(swe::Ipl::Mercury, 20.0),
        std::make_pair(swe::Ipl::Mars, 26.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
});

constexpr auto slowUB = std::to_array<std::pair<swe::Ipl, double>>({
    {swe::Ipl::Moon, 12.30},
    {swe::Ipl::Mercury, 1.00},
    {swe::Ipl::Venus, 0.50},
    {swe::Ipl::Mars, 0.30},
    {swe::Ipl::Jupiter, 0.05},
    {swe::Ipl::Saturn, 0.02},
});

constexpr auto swiftLB = std::to_array<std::pair<swe::Ipl, double>>({
    {swe::Ipl::Moon, 13.30},
    {swe::Ipl::Mercury, 1.38},
    {swe::Ipl::Venus, 1.20},
    {swe::Ipl::Mars, 0.40},
    {swe::Ipl::Jupiter, 0.10},
    {swe::Ipl::Saturn, 0.05},
});

enum class DigDeb {
  Swift,
  Slow,
  Retrograde,
  Direct,
  Oriental,
  Occidental,
  UnderSunBeams,
  Combust,
  Cazimi,
  FreeFromSunBeams,
  Domicile,
  Detriment,
  InOwnTriplicity,
  InOwnFace,
  InOwnTerm,
  Exalted,
  Fallen,
  Peregrine
};

constexpr std::string_view to_string(DigDeb dd) {
  switch (dd) {
    using enum DigDeb;
  case Swift:
    return "Swift";
  case Slow:
    return "Slow";
  case Retrograde:
    return "Retrograde";
  case Direct:
    return "Direct";
  case Oriental:
    return "Oriental";
  case Occidental:
    return "Occidental";
  case UnderSunBeams:
    return "UnderSunBeams";
  case Combust:
    return "Combust";
  case Cazimi:
    return "Cazimi";
  case FreeFromSunBeams:
    return "FreeFromSunBeams";
  case Domicile:
    return "Domicile";
  case Detriment:
    return "Detriment";
  case InOwnTriplicity:
    return "InOwnTriplicity";
  case InOwnFace:
    return "InOwnFace";
  case InOwnTerm:
    return "InOwnTerm";
  case Exalted:
    return "Exalted";
  case Fallen:
    return "Fallen";
  case Peregrine:
    return "Peregrine";
  default:
    throw std::invalid_argument("invalid DigDeb for to_string(...)");
  }
}

template <core::swe::HasEcliptic EC> auto vlon(const EC &ec) {
  return ec.ecliptic().at(3);
}

// template <typename T, T t, typename Comp>
template <auto t, typename Comp>
auto fastMapCmp(swe::Ipl ipl, double x, Comp cmp) {
  static constexpr auto map = FastMap<swe::Ipl, double, t.size()>{{t}};
  if (auto found = map.at(ipl)) {
    return cmp(x, *found);
  } else {
    return false;
  }
}

using DigDebCb = std::pair<DigDeb, std::function<bool(const swe::Planet &)>>;

const auto digDebCallTable = std::to_array<DigDebCb>({
    {DigDeb::Swift,
     [](const auto &pl) {
       return fastMapCmp<swiftLB>(pl.id(), pl.ecliptic().at(3),
                                  std::greater_equal<>{});
     }},
    {DigDeb::Slow,
     [](const auto &pl) {
       return fastMapCmp<slowUB>(pl.id(), pl.ecliptic().at(3),
                                 std::less_equal<>{});
     }},
    {DigDeb::Retrograde,
     [](const auto &pl) {
       return pl.id() != swe::Ipl::Sun && pl.id() != swe::Ipl::Moon &&
              vlon(pl) < 0.0;
     }},
    {DigDeb::Direct,
     [](const auto &pl) {
       return pl.id() != swe::Ipl::Sun && pl.id() != swe::Ipl::Moon &&
              vlon(pl) > 0.0;
     }},
    {DigDeb::Domicile,
     [](const auto &pl) {
       return pl.id() == houseOrder.at(pl.ecliptic().at(0) / 30);
     }},
    {DigDeb::Detriment,
     [](const auto &pl) {
       return pl.id() ==
              houseOrder.at((static_cast<int>(pl.ecliptic().at(0) / 30) + 6) %
                            12);
     }},
    {DigDeb::InOwnFace,
     [](const auto &pl) {
       return pl.id() ==
              core::faceOrder.at(
                  static_cast<std::size_t>(pl.ecliptic().at(0) / 10.0) % 7);
     }},
    {DigDeb::InOwnTerm,
     [](const auto &pl) {
       const auto d = std::fmod(pl.ecliptic().at(0), 30);
       const TermsPentad &sign = terms.at(pl.ecliptic().at(0) / 30);
       return std::ranges::adjacent_find(
                  sign, [&](const auto &x, const auto &y) {
                    return d >= x.second && d < y.second && y.first == pl.id();
                  }) != sign.end();
     }},
    {DigDeb::Exalted,
     [](const auto &pl) {
       return fastMapCmp<core::exaltations>(pl.id(), pl.ecliptic().at(0),
                                            std::equal_to<>{});
     }},
    {DigDeb::Fallen,
     [](const auto &pl) {
       return fastMapCmp<core::exaltations>(
           pl.id(), swe_degnorm(pl.ecliptic().at(0) + 180.0),
           std::equal_to<>{});
     }},
});

consteval auto dms2double(double degs, double mins, double secs) {
  return degs + mins / 60.0 + secs / 3600.0;
}

constexpr auto orbAbs(const auto &a, const auto &b) {
  return std::abs(swe_difdeg2n(a.ecliptic().at(0), b.ecliptic().at(0)));
}

DigDeb sunAccDeb(const auto &sun, const auto &a) {
  constinit static auto orbCombust = dms2double(8, 30, 0);
  constinit static auto orbUnderSunBeams = dms2double(17, 0, 0);
  constinit static auto orbCazimi = dms2double(0, 17, 0);
  const auto orb = orbAbs(sun, a);
  if (orb > orbUnderSunBeams)
    return DigDeb::FreeFromSunBeams;
  if (orb <= orbCazimi)
    return DigDeb::Cazimi;
  if (orb <= orbCombust)
    return DigDeb::Combust;

  return DigDeb::UnderSunBeams;
}

auto houseScore(std::size_t house_num) {
  constinit static auto _ =
      std::to_array<char>({0, 5, 3, 1, 4, 3, -2, 4, -2, 2, 5, 4, -5});
  assert(house_num >= 0 && house_num <= 12);
  return _.at(house_num);
}

auto sunPlanetRise(const Chart &c, const auto &a) {
  double tret[3];
  double tret2[3];

  double geo[3] = {c.coord.at(0), c.coord.at(1), 0.0};

  swe::swe_call(swe_rise_trans, c.ut,
                static_cast<std::underlying_type_t<swe::Ipl>>(a.id()), nullptr,
                0, SE_CALC_RISE, geo, 0, 0, tret);

  swe::swe_call(swe_rise_trans, c.ut,
                static_cast<std::underlying_type_t<swe::Ipl>>(swe::Ipl::Sun),
                nullptr, 0, SE_CALC_RISE, geo, 0, 0, tret2);

  return tret[0] < tret2[0] ? DigDeb::Oriental : DigDeb::Occidental;
}

auto isInOwnTriplicity(bool is_night, const auto &a) {
  const auto &rulers = triplicityRulers.at(a.ecliptic().at(0) / 30);
  return (is_night ? std::get<1>(rulers) : std::get<0>(rulers)) == a.id();
}

} // namespace specni::core

namespace specni::gui {
// not defined in std for some reason?
template <typename T>
concept arithmetic = std::is_arithmetic<T>::value;
template <arithmetic T> constexpr double deg2rad(T deg) {
  return deg * (std::numbers::pi / 180.0);
}

template <arithmetic T>
std::tuple<std::valarray<T>, std::valarray<T>> cosSin(std::valarray<T> &x) {
  return std::make_pair(std::cos(x), std::sin(x));
}
} // namespace specni::gui

namespace ImGuiExtra {
template <typename... Args>
constexpr void Text(fmt::format_string<Args...> f, Args &&...args) {
  ImGui::TextUnformatted(fmt::format(f, std::forward<Args>(args)...).c_str());
}
} // namespace ImGuiExtra

namespace specni::gui {

template <std::forward_iterator It, class BinaryFunction>
constexpr void ring_adjacency(It first, It last, BinaryFunction func) {
  if (first == last)
    return;
  It next = first;
  It first2 = first;
  while (++next != last) {
    func(*first, *next);
    first = next;
  }
  func(*first2, *first);
}

void ShowChart(core::Chart &model, std::array<ImFont *, 2> &fonts) {
  ColorPalette colors{ImColor(0.5f, 0.5f, 0.5f), ImColor(0.2f, 0.2f, 0.5f)};
  enum Ratio {
    Innermost,
    SignInner,
    SignOuter,
    CuspText,
    CircleHouseNumbers,
    Count
  };

  static constexpr std::array<double, Count> CirclesRadii = {
      0.25f, 0.40f, 0.45f, 0.40f, 0.28f};

  static constexpr float Thickness = 1.0f;

  const int PlanetWidgetTableFlags =
      ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
      ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter |
      ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg;

  static constexpr std::string_view str =
      "Equal\0Alcabitius\0Campanus\0EqualMC\0Carter\0Gauquelin\0Azimuth\0Sunshi"
      "ne\0SunshineAlt\0Koch\0PullenSDelta\0Morinus\0WholeSign\0Porphyry\0Placi"
      "dus\0PullenSRatio\0Regiomontanus\0Sripati\0PolichPage\0KrusinskiPisaGoel"
      "zer\0EqualVehlow\0EqualWholeSign\0ARSMeridian\0APC\0";

  auto hsys_from_index = [](std::size_t x) {
    return static_cast<core::swe::HouseSystem>(
        (char)((x > 3) ? (66 + x) : (65 + x)));
  };

  ImGui::Begin("Controls");

  static double latitude;
  static double longitude;

  auto now = std::chrono::system_clock::now();
  const auto ymd =
      std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(now)};

  const auto hms = std::chrono::hh_mm_ss{
      now.time_since_epoch() -
      static_cast<std::chrono::sys_days>(ymd).time_since_epoch()};

  static specni::core::swe::DateTime dt{
      static_cast<int>(ymd.year()),
      static_cast<unsigned int>(ymd.month()),
      static_cast<unsigned int>(ymd.day()),
      static_cast<unsigned int>(hms.hours().count()),
      static_cast<unsigned int>(hms.minutes().count()),
      static_cast<unsigned int>(hms.seconds().count())};

  ImGui::PushItemWidth(-1);

  float Avail = ImGui::CalcItemWidth();
  float LabelW = ImGui::CalcTextSize("Month").x;
  ImGui::PopItemWidth();
  ImGui::PushItemWidth((Avail / 3) - LabelW -
                       ImGui::GetStyle().ItemInnerSpacing.x);
  ImGui::DragInt("Year", &dt.Year);

  ImGui::SameLine();

  static constexpr unsigned int Min = 1;
  static constexpr unsigned int MaxMonth = 12;
  static constexpr unsigned int MaxMinSec = 59;
  static constexpr unsigned int MaxHours = 23;

  ImGui::DragScalar("Month", ImGuiDataType_U32, &dt.Month, 1.0f, &Min,
                    &MaxMonth, "%u", ImGuiSliderFlags_AlwaysClamp);

  auto dayMax = static_cast<unsigned int>(std::chrono::year_month_day_last{
      std::chrono::year(dt.Year) / std::chrono::month(dt.Month) /
      std::chrono::last}
                                              .day());
  if (dt.Day > dayMax) {
    dt.Day = dayMax;
  }

  ImGui::SameLine();

  ImGui::DragScalar("Day", ImGuiDataType_U32, &dt.Day, 1.0f, &Min, &dayMax,
                    "%u", ImGuiSliderFlags_AlwaysClamp);

  ImGui::DragScalar("Hours", ImGuiDataType_U32, &dt.Hours, 1.0f, &Min,
                    &MaxHours, "%u", ImGuiSliderFlags_AlwaysClamp);

  ImGui::SameLine();

  ImGui::DragScalar("Mins", ImGuiDataType_U32, &dt.Minutes, 1.0f, &Min,
                    &MaxMinSec, "%u", ImGuiSliderFlags_AlwaysClamp);

  ImGui::SameLine();

  ImGui::DragScalar("Secs", ImGuiDataType_U32, &dt.Seconds, 1.0f, &Min,
                    &MaxMinSec, "%u", ImGuiSliderFlags_AlwaysClamp);

  ImGui::PopItemWidth();

  ImGui::PushItemWidth(-1);
  Avail = ImGui::CalcItemWidth();
  LabelW = ImGui::CalcTextSize("Longitude").x;
  ImGui::PopItemWidth();
  ImGui::PushItemWidth((Avail / 2) - LabelW -
                       ImGui::GetStyle().ItemInnerSpacing.x);

  ImGui::InputDouble("Latitude", &latitude);

  ImGui::SameLine();

  ImGui::InputDouble("Longitude", &longitude);

  ImGui::PopItemWidth();

  static int houseSel = 0;
  ImGui::Combo("House System", &houseSel, str.data());

  model.update(dt, {longitude, latitude}, hsys_from_index(houseSel));

  ImGui::End();

  ImGui::Begin("Planets");
  if (ImGui::BeginTable("split2", 5, PlanetWidgetTableFlags)) {
    for (auto x : {"Body", "Location", "Velocity", "House", "Decl"}) {
      ImGui::TableSetupColumn(x);
    }

    ImGui::TableHeadersRow();

    std::ranges::for_each(model.planets, [&](const auto &p) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGuiExtra::Text("{}", p.name().data());
      ImGui::TableNextColumn();

      ImGui::Text("%lf", p.ecliptic().at(0));
      ImGui::TableNextColumn();

      ImGui::Text("%lf", p.ecliptic().at(3));
      ImGui::TableNextColumn();

      ImGui::Text("%d",
                  static_cast<unsigned>(std::trunc(model.houses.housePos(p))));
      ImGui::TableNextColumn();

      ImGui::Text("%lf", p.equatorial().at(1));
      ImGui::TableNextColumn();
    });

    ImGui::EndTable();
  }

  ImGui::End();

  ImGui::Begin("Aspects");

  if (ImGui::BeginTable("aspects", 3, PlanetWidgetTableFlags)) {
    for_each_combination(
        std::begin(model.planets), std::next(std::begin(model.planets), 2),
        std::end(model.planets),
        [](const auto &x, const auto &y) {
          if (auto it = specni::core::maybe_aspect(*x, *std::next(x))) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", x->name().data());

            ImGui::TableNextColumn();
            ImGui::Text("%d", *it);

            ImGui::TableNextColumn();
            ImGui::Text("%s", std::next(x)->name().data());
          }

          return false;
        }

    );

    ImGui::EndTable();
  }

  ImGui::End();

  ImGui::Begin("Houses");

  if (ImGui::BeginTable("houses_table", 2, PlanetWidgetTableFlags)) {
    ImGui::TableSetupColumn("House");
    ImGui::TableSetupColumn("Sign/DMS");
    ImGui::TableHeadersRow();
    std::for_each(std::begin(model.houses.cusps) + 1,
                  std::end(model.houses.cusps), [i = 0](auto cusp) mutable {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("House %d", (++i));
                    ImGui::TableNextColumn();
                    ImGui::Text("%lf", cusp);
                  });

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Asc");
    ImGui::TableNextColumn();
    ImGui::Text("%lf", model.houses.ang.at(core::swe::AC));

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Mc");
    ImGui::TableNextColumn();
    ImGui::Text("%lf", model.houses.ang.at(core::swe::MC));

    ImGui::EndTable();
  }

  ImGui::End();

  ImGui::Begin("Scores");

  auto sun = std::ranges::find_if(model.planets, [](const auto &pl) {
    return pl.id() == core::swe::Ipl::Sun;
  });

  std::ranges::for_each(model.planets, [&](const core::swe::Planet &pl) {
    if (ImGui::TreeNode(pl.name().c_str())) {
      std::ranges::for_each(core::digDebCallTable, [&](const auto &x) {
        if (x.second(pl))
          ImGuiExtra::Text("{}", to_string(x.first));
      });
      if (sun != std::cend(model.planets) && sun->id() != pl.id()) {
        if (auto is_night =
                [](const auto &sun, auto ac) {
                  return swe_difdeg2n(sun.ecliptic().at(0), ac) >= 0;
                }(*sun, model.houses.ang.at(0));
            core::isInOwnTriplicity(is_night, pl)) {
          ImGuiExtra::Text("{}", to_string(core::DigDeb::InOwnTriplicity));
        }

        ImGuiExtra::Text("{}", to_string(core::sunAccDeb(*sun, pl)));
        ImGuiExtra::Text("{}", to_string(core::sunPlanetRise(model, pl)));
      }
      ImGui::TreePop();
    }
  });

  ImGui::End();

  ImGui::SetNextWindowSizeConstraints(
      ImVec2(600, 600), ImVec2(800, 800), [](ImGuiSizeCallbackData *data) {
        auto max = std::max(data->DesiredSize.x, data->DesiredSize.y);
        data->DesiredSize = ImVec2(max, max);
      });

  ImGui::Begin("Chart");
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  // const ImVec2 p = ImGui::GetCursorScreenPos();
  ImVec2 window_pos = ImGui::GetWindowPos();
  ImVec2 window_size = ImGui::GetWindowSize();

  constexpr double half = 0.5f;
  ImVec2 window_center = ImVec2(window_pos.x + window_size.x * half,
                                window_pos.y + window_size.y * half);

  const auto regions = [&]() {
    std::array<ImVec2, Count> regions;
    std::ranges::transform(CirclesRadii, regions.begin(), [&](auto x) {
      return ImVec2(0, window_size.y * x);
    });
    return regions;
  }();

  const auto center = [](const ImVec2 &a, const ImVec2 &b) {
    return (a + b) / 2.0;
  };

  ImVec2 RAscMc(center(regions[CuspText], regions[SignOuter]));

  ImVec2 RHouseNumber(center(regions[Innermost], regions[CircleHouseNumbers]));

  ImVec2 RSign(center(regions[SignInner], regions[SignOuter]));

  // Draw circles
  std::for_each_n(regions.begin(), 5, [&](auto x) {
    draw_list->AddCircle(window_center, x.y, colors[1], 0, Thickness);
  });

  constexpr int SignCount = 12;
  constexpr int SignSpanDegrees = 30;
  constexpr double HalfSignSpanRad = deg2rad(SignSpanDegrees / 2);

  std::valarray<double> sc(SignCount * 2);

  auto translate = deg2rad(model.houses.ang.at(core::swe::Angle::AC) + 90.0);

  std::generate(std::begin(sc), std::end(sc),
                [rad = translate]() mutable { return rad -= HalfSignSpanRad; });

  auto [vcos, vsin] = cosSin(sc);

  static constexpr std::array<std::string_view, SignCount> glyphs = {
      "a"sv, "b"sv, "c"sv, "d"sv, "e"sv, "f"sv,
      "g"sv, "h"sv, "i"sv, "j"sv, "k"sv, "l"sv};

  auto line_r = [](int i) { return i * 2 + 1; };
  auto glyph_r = [](int i) { return i * 2; };

  auto cos_sin = [&](double deg) {
    auto rad = translate - deg2rad(deg);
    return std::make_pair(std::cos(rad), std::sin(rad));
  };

  ImGui::PushFont(fonts[1]);
  for (int i = 0; i < SignCount; i++) {
    draw_list->AddLine(
        window_center +
            ImRotate(regions[SignInner], vcos[line_r(i)], vsin[line_r(i)]),
        window_center +
            ImRotate(regions[SignOuter], vcos[line_r(i)], vsin[line_r(i)]),
        colors[1], Thickness);

    draw_list->AddText(window_center +
                           ImRotate(RSign, vcos[glyph_r(i)], vsin[glyph_r(i)]),
                       colors[1], glyphs[i].data());
  }

  ImVec2 r_planets(0, window_size.y * 0.38f);
  std::ranges::for_each(model.planets, [&](const auto &pl) {
    constexpr auto m =
        FastMap<core::swe::Ipl, std::pair<std::string_view, ImVec4>,
                PlanetGlyphMap.size()>{{PlanetGlyphMap}};

    auto val = m.at(pl.id());
    if (val.has_value()) {
      auto angle = translate - deg2rad(pl.ecliptic().at(0));
      draw_list->AddText(
          window_center + ImRotate(r_planets, std::cos(angle), std::sin(angle)),
          ImColor(val->second), val->first.data());
    }
  });

  constexpr std::array<std::string_view, 2> lglyph = {"K", "L"};
  auto draw_angle = [&](double deg, std::size_t glyph_ind) {
    auto [CosA, SinA] = cos_sin(deg);
    draw_list->AddLine(window_center + ImRotate(regions[SignOuter], CosA, SinA),
                       window_center + ImRotate(regions[Innermost], CosA, SinA),
                       colors[0], Thickness);

    draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA), colors[0],
                       lglyph[glyph_ind].data());
  };

  draw_angle(model.houses.ang.at(core::swe::AC), 0);
  draw_angle(model.houses.ang.at(core::swe::MC), 1);

  std::for_each(model.houses.cusps.cbegin() + 1, model.houses.cusps.cend(),
                [&](float cuspStart) {
                  auto [CosA, SinA] = cos_sin(cuspStart);
                  draw_list->AddLine(
                      window_center + ImRotate(regions[Innermost], CosA, SinA),
                      window_center + ImRotate(regions[SignInner], CosA, SinA),
                      colors[0], Thickness);
                });

  ImGui::PopFont();

  // For printing house number glyphs
  static constexpr auto house_num_glyph = std::to_array<std::string_view>(
      {"1"sv,  "2"sv,  "3"sv,  "4"sv,  "5"sv,  "6"sv,  "7"sv,  "8"sv,
       "9"sv,  "10"sv, "11"sv, "12"sv, "13"sv, "14"sv, "15"sv, "16"sv,
       "17"sv, "18"sv, "19"sv, "20"sv, "21"sv, "22"sv, "23"sv, "24"sv,
       "25"sv, "26"sv, "27"sv, "28"sv, "29"sv, "30"sv, "31"sv, "32"sv,
       "33"sv, "34"sv, "35"sv, "36"sv, "37"sv});

  ImGui::PushFont(fonts[0]);
  ring_adjacency(
      std::cbegin(model.houses.cusps) + 1, std::cend(model.houses.cusps),
      [&, i = 0](const auto &x, const auto &y) mutable {
        auto [CosA, SinA] = cos_sin(swe_deg_midp(x, y));
        draw_list->AddText(window_center + ImRotate(RHouseNumber, CosA, SinA),
                           colors[0], house_num_glyph[i++].data());
      });
  ImGui::PopFont();

  ImGui::End();
}
} // namespace specni::gui
