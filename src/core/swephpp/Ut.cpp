#include "Ut.hpp"

#include <cassert>

// clang-format off
#include "swephexp.h"
// clang-format on

namespace specni {
namespace core {
namespace swe {

Ut::Ut(const GregorianTime &dt) {
  constexpr int GregorianFlag = 1;
  constexpr int UT1 = 1;
  std::array<double, 2> dret;
  std::array<char, 256> serr;
  assert(swe_utc_to_jd(dt.year, dt.month, dt.day, dt.hour, dt.min, dt.sec,
                       GregorianFlag, dret.data(), serr.data()) == OK);
  m = dret[UT1];
}

}; // namespace swe
}; // namespace core
}; // namespace specni
