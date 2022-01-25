#pragma once

#include "core/swephpp.hpp"
#include <string>
#include <vector>

#include <core/ChartModel.hpp>
#include <core/PlanetStates.hpp>

namespace specni {
using DignityTuple = std::tuple<char, std::string>;
using DignityScoreTable = std::unordered_map<swephpp::Ipl, std::vector<DignityTuple>>;

auto GetDignityScoreTable(const ChartModel &model) -> DignityScoreTable;
}; // namespace specni