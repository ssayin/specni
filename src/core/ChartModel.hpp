#pragma once

#include <core/swephpp.hpp>

namespace specni {
namespace core {
class ChartModel {
public:
  ChartModel(const swe::Ut &ut, const swe::Coordinate &geodetic,
             swe::HouseSystem hs);

private:
  swe::House houses;
  std::unordered_map<swe::Ipl, const swe::Planet &> mPl;
};
}; // namespace core

}; // namespace specni
