#pragma once

#include <array>
#include <cstdint>

#include "Context.hpp"

namespace specni {
namespace core {
namespace swe {

struct GregorianTime {
  uint16_t year;
  uint16_t month;
  uint16_t day;
  uint16_t hour;
  uint16_t min;
  uint16_t sec;
};

class Ut {
public:
  Ut(Ut &&) = default;
  Ut(const GregorianTime &dt);
  operator double() const { return m; }

private:
  double m;
};

}; // namespace swe
}; // namespace core
}; // namespace specni
