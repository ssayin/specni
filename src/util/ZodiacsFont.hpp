#pragma once

#include "core/Aspects.hpp"
#include "imgui.h"
#include <core/swephpp.hpp>
#include <unordered_map>
#include <utility>

static const std::unordered_map<swephpp::PlanetaryBody, std::pair<char, ImVec4>>
    PlanetCharMap = {
        {swephpp::PlanetaryBody::Sun,
         std::make_pair('A', ImVec4(0.984, 0.650, 0, 1.0))},
        {swephpp::PlanetaryBody::Moon,
         std::make_pair('B', ImVec4(0.858, 0.972, 1, 1.0))},
        {swephpp::PlanetaryBody::Mercury,
         std::make_pair('C', ImVec4(0.941, 0.678, 0.439, 1.0))},
        {swephpp::PlanetaryBody::Venus,
         std::make_pair('D', ImVec4(0.596, 0.062, 0.835, 1.0))},
        {swephpp::PlanetaryBody::Mars,
         std::make_pair('E', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Jupiter,
         std::make_pair('F', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Saturn,
         std::make_pair('G', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Uranus,
         std::make_pair('H', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Neptune,
         std::make_pair('I', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Pluto,
         std::make_pair('J', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::TrueNode,
         std::make_pair('M', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Chiron,
         std::make_pair('Q', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Pholus,
         std::make_pair('R', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Ceres,
         std::make_pair('S', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Pallas,
         std::make_pair('T', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Juno,
         std::make_pair('U', ImVec4(1, 0.019, 0.078, 1.0))},
        {swephpp::PlanetaryBody::Vesta,
         std::make_pair('V', ImVec4(1, 0.019, 0.078, 1.0))},
};

static const std::unordered_map<specni::Aspect, char> AspectCharMap = {
    {specni::Aspect::Conjunction, 'm'},
    {specni::Aspect::Square, 'r'},
    {specni::Aspect::Trine, 's'},
    {specni::Aspect::Opposition, 'v'},
};