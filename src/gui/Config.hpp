#pragma once
#include "imgui.h"
#include <array>
#include <core/Aspects.hpp>

namespace specni {
constexpr int PlanetWidgetTableFlags =
    ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
    ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter |
    ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg;

const std::array<ImVec4, static_cast<size_t>(AspectAngle::Count)> aspectColor{
    ImVec4(1, 1, 1, 1), ImVec4(0, 1, 0, 1), ImVec4(1, 0, 0, 1),
    ImVec4(0, 1, 1, 1), ImVec4(1, 0, 0, 1),
};

static const std::unordered_map<swephpp::Ipl, std::pair<char, ImVec4>>
    PlanetCharMap = {
        {swephpp::Ipl::Sun, std::make_pair('A', ImVec4(0.984, 0.650, 0, 1.0))},
        {swephpp::Ipl::Moon, std::make_pair('B', ImVec4(0.858, 0.972, 1, 1.0))},
        {swephpp::Ipl::Mercury,
         std::make_pair('C', ImVec4(0.941, 0.678, 0.439, 1.0))},
        {swephpp::Ipl::Venus,
         std::make_pair('D', ImVec4(0.596, 0.062, 0.835, 1.0))},
        {swephpp::Ipl::Mars, std::make_pair('E', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Jupiter,
         std::make_pair('F', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Saturn,
         std::make_pair('G', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Uranus,
         std::make_pair('H', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Neptune,
         std::make_pair('I', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Pluto,
         std::make_pair('J', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::TrueNode,
         std::make_pair('M', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Chiron,
         std::make_pair('Q', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Pholus,
         std::make_pair('R', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Ceres,
         std::make_pair('S', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Pallas,
         std::make_pair('T', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Juno, std::make_pair('U', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::Ipl::Vesta,
         std::make_pair('V', ImVec4(1, 0.019, 0.078, 1.0))},
};

static const std::unordered_map<specni::AspectAngle, char> AspectCharMap = {
    {specni::AspectAngle::Conjunction, 'm'},
    {specni::AspectAngle::Sextile, 'p'},
    {specni::AspectAngle::Square, 'q'},
    {specni::AspectAngle::Trine, 'r'},
    {specni::AspectAngle::Opposition, 'u'},
};
}; // namespace specni
