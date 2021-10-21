#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <ostream>

#include <cmath>
#include <string>

namespace specni {
template <typename T, int MIN, int MAX> class Cyclic {
public:
  Cyclic() = default;
  virtual ~Cyclic() = default;
  Cyclic(const double o) { this->m = o; }
  Cyclic(const Cyclic &o) { *this = o; }

  Cyclic &operator+=(const Cyclic &rhs);

  Cyclic &operator-=(const Cyclic &rhs);

  friend Cyclic operator+(Cyclic lhs, const Cyclic &rhs) {
    lhs += rhs;
    return lhs;
  }
  friend Cyclic operator-(Cyclic lhs, const Cyclic &rhs) {
    lhs -= rhs;
    return lhs;
  }

  // inline constexpr operator T() const { return this->m; }
  inline constexpr T operator()() const { return this->m; }
  auto operator<=>(const Cyclic &) const = default;

  // DMS
  friend std::ostream &operator<<(std::ostream &os, const Cyclic &obj) {
    /*int sec = static_cast<int>(std::round(obj.m * 3600));
    int deg = sec / 3600;
    sec = std::abs(sec % 3600);
    int min = sec / 60;
    sec %= 60;

    os << deg << "°" << min << "'" << sec << "\"";
    os.flush();
*/
    os << std::to_string(obj.m);
    return os;
  }

  bool within(const Cyclic &a, int orb) {
    return ((a + *this) <= orb) || ((a - *this) <= orb) || ((*this - a) <= orb);
  }

private:
  T m;
};

template <typename T, int MIN, int MAX>
Cyclic<T, MIN, MAX> &Cyclic<T, MIN, MAX>::operator+=(const Cyclic &rhs) {
  this->m += rhs.m;
  while (this->m > MAX)
    this->m -= MAX;
  return *this;
}

template <typename T, int MIN, int MAX>
Cyclic<T, MIN, MAX> &Cyclic<T, MIN, MAX>::operator-=(const Cyclic &rhs) {
  this->m -= rhs.m;
  while (this->m < MIN)
    this->m += MAX;
  return *this;
}

typedef Cyclic<double, -90, 90> Latitude;
typedef Cyclic<double, 0, 360> Longitude;

}; // namespace specni