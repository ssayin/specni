#ifndef SPECNI_ZODIACSFONT_H
#define SPECNI_ZODIACSFONT_H

#include "imgui.h"
#include <core/swephpp.hpp>
#include <unordered_map>
#include <utility>

static const std::unordered_map<specni::core::swe::Ipl, std::pair<char, ImVec4>>
		PlanetCharMap = {
		{specni::core::swe::Ipl::Sun, std::make_pair('A', ImVec4(0.984, 0.650, 0, 1.0))},
		{specni::core::swe::Ipl::Moon, std::make_pair('B', ImVec4(0.858, 0.972, 1, 1.0))},
		{specni::core::swe::Ipl::Mercury,
				std::make_pair('C', ImVec4(0.941, 0.678, 0.439, 1.0))},
		{specni::core::swe::Ipl::Venus,
				std::make_pair('D', ImVec4(0.596, 0.062, 0.835, 1.0))},
		{specni::core::swe::Ipl::Mars, std::make_pair('E', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Jupiter,
				std::make_pair('F', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Saturn,
				std::make_pair('G', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Uranus,
				std::make_pair('H', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Neptune,
				std::make_pair('I', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Pluto,
				std::make_pair('J', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::TrueNode,
				std::make_pair('M', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Chiron,
				std::make_pair('Q', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Pholus,
				std::make_pair('R', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Ceres,
				std::make_pair('S', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Pallas,
				std::make_pair('T', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Juno, std::make_pair('U', ImVec4(1, 0.019, 0.078, 1.0))},
		{specni::core::swe::Ipl::Vesta,
				std::make_pair('V', ImVec4(1, 0.019, 0.078, 1.0))},
};
/*
static const std::unordered_map<specni::Aspect, char> AspectCharMap = {
		{specni::Aspect::Conjunction, 'm'}, {specni::Aspect::Sextile, 'p'},
		{specni::Aspect::Square, 'q'},      {specni::Aspect::Trine, 'r'},
		{specni::Aspect::Opposition, 'u'},
};
*/

#endif //SPECNI_ZODIACSFONT_H
