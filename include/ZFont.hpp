#ifndef SPECNI_ZODIACSFONT_H
#define SPECNI_ZODIACSFONT_H

#include "SwEphpp.hpp"
#include <imgui.h>
#include <utility>

namespace specni {
using namespace std::literals::string_view_literals;

static constexpr ImVec4 Color1 = {0.984, 0.650, 0, 1.0};
static constexpr ImVec4 Color2 = {0.858, 0.972, 1, 1.0};
static constexpr ImVec4 Color3 = {0.941, 0.678, 0.439, 1.0};
static constexpr ImVec4 Color4 = {0.596, 0.062, 0.835, 1.0};
static constexpr ImVec4 Color5 = {1, 0.019, 0.078, 1.0};

using ColorGlyphPair = std::pair<std::string_view, ImVec4>;
using IplDataPair = std::pair<specni::core::swe::Ipl, ColorGlyphPair>;

static constexpr auto PlanetGlyphMap = std::to_array<IplDataPair>({
    {core::swe::Ipl::Sun, std::make_pair("A"sv, Color1)},
    {core::swe::Ipl::Moon, std::make_pair("B"sv, Color2)},
    {core::swe::Ipl::Mercury, std::make_pair("C"sv, Color3)},
    {core::swe::Ipl::Venus, std::make_pair("D"sv, Color4)},
    {core::swe::Ipl::Mars, std::make_pair("E"sv, Color5)},
    {core::swe::Ipl::Jupiter, std::make_pair("F"sv, Color5)},
    {core::swe::Ipl::Saturn, std::make_pair("G"sv, Color5)},
    {core::swe::Ipl::Uranus, std::make_pair("H"sv, Color5)},
    {core::swe::Ipl::Neptune, std::make_pair("I"sv, Color5)},
    {core::swe::Ipl::Pluto, std::make_pair("J"sv, Color5)},
    {core::swe::Ipl::TrueNode, std::make_pair("M"sv, Color5)},
    {core::swe::Ipl::Chiron, std::make_pair("Q"sv, Color5)},
    {core::swe::Ipl::Pholus, std::make_pair("R"sv, Color5)},
    {core::swe::Ipl::Ceres, std::make_pair("S"sv, Color5)},
    {core::swe::Ipl::Pallas, std::make_pair("T"sv, Color5)},
    {core::swe::Ipl::Juno, std::make_pair("U"sv, Color5)},
    {core::swe::Ipl::Vesta, std::make_pair("V"sv, Color5)},
});

enum Aspect { Conjunction, Sextile, Square, Trine, Opposition };

static constexpr auto AspectGlyphMap =
    std::to_array<std::pair<Aspect, std::string_view>>({
        {Conjunction, "m"sv},
        {Sextile, "p"sv},
        {Square, "q"sv},
        {Trine, "r"sv},
        {Opposition, "u"sv},
    });
} // namespace specni

#endif // SPECNI_ZODIACSFONT_H
