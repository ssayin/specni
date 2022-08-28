#include "Wheel.h"
#include "ImGuiExt.hpp"
#include "ZodiacsFont.h"
#include <FastMap.hpp>
#include <algorithm>
#include <imgui.h>
#include <numbers>
#include <numeric>
#include <swephpp.hpp>
#include <tuple>
#include <type_traits>
#include <utility>
#include <valarray>

namespace specni::gui::degree {

// not defined in std for some reason?
template <typename T>
concept arithmetic = std::is_arithmetic<T>::value;
template <arithmetic T> constexpr double toRadians(T deg) {
  return deg * (std::numbers::pi / 180.0);
}

template <arithmetic T> std::tuple<double, double> cosSin(T deg) {
  return std::make_tuple(
      cos(toRadians(-deg)),
      sin(toRadians(-deg))); // we want to draw counter-clockwise
}

template <arithmetic T>
std::tuple<std::valarray<T>, std::valarray<T>> cosSin2(std::valarray<T> &x) {
  return std::make_tuple(std::cos(x), std::sin(x));
}

std::tuple<double, double> cosSinFixAt(double deg, double asc) {
  return cosSin(deg - asc - 90);
}

} // namespace specni::gui::degree

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

void Wheel::Show() const {

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
    std::array<ImVec2, ChartSettings::Count> regions;
    std::transform(std::cbegin(settings.CirclesRadii),
                   std::cend(settings.CirclesRadii), regions.begin(),
                   [&](auto x) { return ImVec2(0, window_size.y * x); });
    return regions;
  }();

  const auto center = [](const ImVec2 &a, const ImVec2 &b) {
    return (a + b) / 2.0;
  };

  ImVec2 RAscMc(center(regions[ChartSettings::CuspText],
                       regions[ChartSettings::SignOuter]));

  ImVec2 RHouseNumber(center(regions[ChartSettings::Innermost],
                             regions[ChartSettings::CircleHouseNumbers]));

  ImVec2 RSign(center(regions[ChartSettings::SignInner],
                      regions[ChartSettings::SignOuter]));

  const auto fromCenter = [&](const ImVec2 &r /* radius, or region */,
                              double cosA, double sinA) {
    return window_center + ImRotate(r, cosA, sinA);
  };

  // Draw circles
  std::for_each_n(regions.begin(), 5, [&](auto x) {
    draw_list->AddCircle(window_center, x.y, settings.colors[1], 0,
                         settings.Thickness);
  });

  ImGui::PushFont(settings.font);

  constexpr int SignCount = 12;
  constexpr int SignSpanDegrees = 30;
  constexpr double HalfSignSpanRad = degree::toRadians(SignSpanDegrees / 2);

  std::valarray<double> sc(SignCount * 2);

  std::generate(std::begin(sc), std::end(sc),
                [rad = degree::toRadians(
                     model.houses.ang.at(core::swe::Angle::AC) +
                     90)]() mutable { return rad -= HalfSignSpanRad; });

  auto [vcos, vsin] = degree::cosSin2(sc);

  static constexpr std::array<std::string_view, SignCount> glyphs = {
      "a"sv, "b"sv, "c"sv, "d"sv, "e"sv, "f"sv,
      "g"sv, "h"sv, "i"sv, "j"sv, "k"sv, "l"sv};

  auto line_r = [](int i) { return i * 2 + 1; };
  auto glyph_r = [](int i) { return i * 2; };

  for (int i = 0; i < SignCount; i++) {
    draw_list->AddLine(
        window_center + ImRotate(regions[ChartSettings::SignInner],
                                 vcos[line_r(i)], vsin[line_r(i)]),
        window_center + ImRotate(regions[ChartSettings::SignOuter],
                                 vcos[line_r(i)], vsin[line_r(i)]),
        settings.colors[1], settings.Thickness);

    draw_list->AddText(window_center +
                           ImRotate(RSign, vcos[glyph_r(i)], vsin[glyph_r(i)]),
                       settings.colors[1], glyphs[i].data());
  }

  ImVec2 mid2 = ImVec2(0, window_size.y * 0.32f);
  std::for_each(
      std::cbegin(model.planets), std::cend(model.planets),
      [&](const auto &pl) {
        constexpr auto m =
            FastMap<core::swe::Ipl, std::pair<std::string_view, ImVec4>,
                    PlanetCharMap.size()>{{PlanetCharMap}};

        auto val = m.at(pl.id());
        if (val.has_value()) {
          auto [CosA, SinA] = degree::cosSinFixAt(
              pl.ecliptic().at(0), model.houses.ang.at(core::swe::Angle::AC));
          draw_list->AddText(window_center + ImRotate(mid2, CosA, SinA),
                             ImColor(val->second), val->first.data());
        }
      });

  constexpr std::array<std::string_view, 2> lglyph = {"K", "L"};

  std::valarray<double> angles = {model.houses.ang.at(core::swe::Angle::AC),
                                  model.houses.ang.at(core::swe::Angle::MC)};

  auto [CosA, SinA] =
      degree::cosSinFixAt(model.houses.ang.at(core::swe::Angle::AC),
                          model.houses.ang.at(core::swe::Angle::AC));

  draw_list->AddLine(
      fromCenter(regions[ChartSettings::SignOuter], CosA, SinA),
      window_center + ImRotate(regions[ChartSettings::Innermost], CosA, SinA),
      settings.colors[0], settings.Thickness);

  draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA),
                     settings.colors[0], lglyph[0].data());

  std::tie(CosA, SinA) =
      degree::cosSinFixAt(model.houses.ang.at(core::swe::Angle::MC),
                          model.houses.ang.at(core::swe::Angle::AC));

  draw_list->AddLine(
      window_center + ImRotate(regions[ChartSettings::SignOuter], CosA, SinA),
      window_center + ImRotate(regions[ChartSettings::Innermost], CosA, SinA),
      settings.colors[0], settings.Thickness);

  draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA),
                     settings.colors[0], lglyph[1].data());

  std::vector<ImVec2> vMidpoints;

  std::for_each(
      model.houses.cusps.cbegin() + 1, model.houses.cusps.cend(),
      [&](float cuspStart) {
        auto [CosA, SinA] = degree::cosSinFixAt(
            cuspStart, model.houses.ang.at(core::swe::Angle::AC));
        draw_list->AddLine(
            window_center +
                ImRotate(regions[ChartSettings::Innermost], CosA, SinA),
            window_center +
                ImRotate(regions[ChartSettings::SignInner], CosA, SinA),
            settings.colors[0], settings.Thickness);

        vMidpoints.push_back(window_center +
                             ImRotate(RHouseNumber, CosA, SinA));
      });

  static constexpr std::array<std::string_view, 37> house_num_glyph = {
      "1"sv,  "2"sv,  "3"sv,  "4"sv,  "5"sv,  "6"sv,  "7"sv,  "8"sv,
      "9"sv,  "10"sv, "11"sv, "12"sv, "13"sv, "14"sv, "15"sv, "16"sv,
      "17"sv, "18"sv, "19"sv, "20"sv, "21"sv, "22"sv, "23"sv, "24"sv,
      "25"sv, "26"sv, "27"sv, "28"sv, "29"sv, "30"sv, "31"sv, "32"sv,
      "33"sv, "34"sv, "35"sv, "36"sv, "37"sv};

  ring_adjacency(std::cbegin(vMidpoints), std::cend(vMidpoints),
                 [&, i = 0](const auto &x, const auto &y) mutable {
                   draw_list->AddText(center(x, y), settings.colors[0],
                                      house_num_glyph[i++].data());
                 });

  ImGui::PopFont();
  // draw aspects here
  ImGui::End();
}

Wheel::Wheel(ChartSettings settings, core::Chart &model)
    : settings(settings), model(model) {}

} // namespace specni::gui
