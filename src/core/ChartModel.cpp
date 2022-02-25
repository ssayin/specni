#include "ChartModel.hpp"

namespace specni {
namespace core {

ChartModel::ChartModel(const swe::Ut &ut, const swe::Coordinate &geodetic,
                       swe::HouseSystem hs)
    : houses(ut, swe::ZodiacType::Tropical, geodetic, hs) {

  for (int i = 0; i < 10; ++i) {

    mPl.insert(
        {static_cast<swe::Ipl>(i),
         swe::Planet{static_cast<swe::Ipl>(i), ut, swe::EphFlag::Speed}});
  }
}

}; // namespace core

}; // namespace specni
