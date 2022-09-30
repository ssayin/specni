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

namespace ImGuiExtra {
template <typename... Args>
constexpr void Text(fmt::format_string<Args...> f, Args &&...args) {
  ImGui::TextUnformatted(fmt::format(f, std::forward<Args>(args)...).c_str());
}
} // namespace ImGuiExtra

namespace specni::gui {
static constexpr std::size_t ColorCount = 5;
using ColorPalette = std::array<ImColor, ColorCount>;

// not defined in std for some reason?
template <typename T>
concept arithmetic = std::is_arithmetic<T>::value;
template <arithmetic T> constexpr double deg2rad(T deg) {
  return deg * (std::numbers::pi / 180.0);
}

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

void ShowScores(core::Chart &model, const std::array<ImFont *, 2> &fonts) {
  ImGui::Begin("Scores");

  ImGui::End();
} // namespace specni::gui
void ShowControls(core::Chart &model, const std::array<ImFont *, 2> &fonts) {
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
}

void ShowChart(core::Chart &model, const std::array<ImFont *, 2> &fonts) {

  ShowControls(model, fonts);

  ShowScores(model, fonts);

  ColorPalette colors{ImColor(0.5f, 0.5f, 0.5f), ImColor(0.2f, 0.2f, 0.5f)};

  static constexpr float Thickness = 1.0f;

  const int PlanetWidgetTableFlags =
      ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
      ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter |
      ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg;

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

  const auto CirclesRadii = std::to_array<ImVec2>(
      {ImVec2(0, 0.25f * window_size.y), ImVec2(0, 0.40f * window_size.y),
       ImVec2(0, 0.45f * window_size.y), ImVec2(0, 0.28f * window_size.y)});

  // Draw circles
  std::ranges::for_each(CirclesRadii, [&](const auto &x) {
    draw_list->AddCircle(window_center, x.y, colors[1], 0, Thickness);
  });

  constexpr int SignCount = 12;
  constexpr int SignSpanDegrees = 30;
  constexpr double HalfSignSpanRad = deg2rad(SignSpanDegrees / 2);

  auto translate = deg2rad(model.houses.ang.at(core::swe::Angle::AC) + 90.0);

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

    auto [CosA, SinA] = cos_sin((i + 1) * 30);
    auto [CosA2, SinA2] = cos_sin(30 * i + 15);
    draw_list->AddLine(window_center + ImRotate(CirclesRadii[1], CosA, SinA),
                       window_center + ImRotate(CirclesRadii[2], CosA, SinA),
                       colors[1], Thickness);

    draw_list->AddText(
        window_center +
            ImRotate((CirclesRadii[1] + CirclesRadii[2]) / 2, CosA2, SinA2),
        colors[1], glyphs[i].data());
  }

  ImVec2 r_planets(0, window_size.y * 0.38f);
  std::ranges::for_each(model.planets, [&](const auto &pl) {
    constexpr auto m =
        FastMap<core::swe::Ipl, std::pair<std::string_view, ImVec4>,
                PlanetGlyphMap.size()>{{PlanetGlyphMap}};

    if (auto val = m.at(pl.id()); val.has_value()) {
      auto angle = translate - deg2rad(pl.ecliptic().at(0));
      draw_list->AddText(
          window_center + ImRotate(r_planets, std::cos(angle), std::sin(angle)),
          ImColor(val->second), val->first.data());
    }
  });
  constexpr std::array<std::string_view, 2> lglyph = {"K", "L"};
  auto draw_angle = [&](double deg, std::size_t glyph_ind) {
    auto [CosA, SinA] = cos_sin(deg);
    draw_list->AddLine(window_center + ImRotate(CirclesRadii[0], CosA, SinA),
                       window_center + ImRotate(CirclesRadii[2], CosA, SinA),
                       colors[0], Thickness);

    draw_list->AddText(window_center + ImRotate(CirclesRadii[2], CosA, SinA),
                       colors[0], lglyph[glyph_ind].data());
  };

  draw_angle(model.houses.ang.at(core::swe::AC), 0);
  draw_angle(model.houses.ang.at(core::swe::MC), 1);

  std::for_each(model.houses.cusps.cbegin() + 1, model.houses.cusps.cend(),
                [&](float cuspStart) {
                  auto [CosA, SinA] = cos_sin(cuspStart);
                  draw_list->AddLine(
                      window_center + ImRotate(CirclesRadii[0], CosA, SinA),
                      window_center + ImRotate(CirclesRadii[1], CosA, SinA),
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
        draw_list->AddText(
            window_center +
                ImRotate((CirclesRadii[0] + CirclesRadii[3]) / 2, CosA, SinA),
            colors[0], house_num_glyph[i++].data());
      });
  ImGui::PopFont();

  core::for_each_aspect(model.planets, [&](const auto &p1, const auto &p2,
                                           auto asp) {
    auto [CosA, SinA] = cos_sin(p1.ecliptic().at(0));
    auto [CosA2, SinA2] = cos_sin(p2.ecliptic().at(0));

    draw_list->AddLine(window_center + ImRotate(CirclesRadii[0], CosA, SinA),
                       window_center + ImRotate(CirclesRadii[0], CosA2, SinA2),
                       colors[0], Thickness);
  });

  ImGui::End();
}
} // namespace specni::gui
