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
  constexpr Cyclic() = default;
  constexpr virtual ~Cyclic() = default;
  constexpr Cyclic(const double o) { this->m = o; }
  constexpr Cyclic(const Cyclic &o) { *this = o; }

  constexpr Cyclic &operator+=(const Cyclic &rhs);

  constexpr Cyclic &operator-=(const Cyclic &rhs);

  constexpr friend Cyclic operator+(Cyclic lhs, const Cyclic &rhs) {
    lhs += rhs;
    return lhs;
  }
  constexpr friend Cyclic operator-(Cyclic lhs, const Cyclic &rhs) {
    lhs -= rhs;
    return lhs;
  }

  // inline constexpr operator T() const { return this->m; }
  inline constexpr T operator()() const { return this->m; }
  constexpr auto operator<=>(const Cyclic &) const = default;

  constexpr friend std::ostream &operator<<(std::ostream &os,
                                            const Cyclic &obj) {
    os << std::to_string(obj.m);
    return os;
  }

  constexpr bool within(const Cyclic &a, double orb) {
    return ((a - *this) <= orb) || ((*this - a) <= orb);
  }

private:
  T m;
};

template <typename T, int MIN, int MAX>
constexpr Cyclic<T, MIN, MAX> &
Cyclic<T, MIN, MAX>::operator+=(const Cyclic &rhs) {
  this->m += rhs.m;
  while (this->m > MAX)
    this->m -= MAX;
  return *this;
}

template <typename T, int MIN, int MAX>
constexpr Cyclic<T, MIN, MAX> &
Cyclic<T, MIN, MAX>::operator-=(const Cyclic &rhs) {
  this->m -= rhs.m;
  while (this->m < MIN)
    this->m += MAX;
  return *this;
}

typedef Cyclic<double, -90, 90> Latitude;
typedef Cyclic<double, 0, 360> Longitude;

}; // namespace specni