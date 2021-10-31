#pragma once

#include "core/swephpp.hpp"
#include <string>
#include <vector>

#include <core/ChartModel.hpp>
#include <core/PlanetStates.hpp>

namespace specni {
typedef std::tuple<char, std::string> DignityTuple;
typedef std::unordered_map<swephpp::Ipl, std::vector<DignityTuple>>
    DignityScoreTable;

DignityScoreTable GetDignityScoreTable(const ChartModel &model);
}; // namespace specni