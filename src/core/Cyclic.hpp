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

  constexpr auto operator+=(const Cyclic &rhs) -> Cyclic &;

  constexpr auto operator-=(const Cyclic &rhs) -> Cyclic &;

  constexpr friend auto operator+(Cyclic lhs, const Cyclic &rhs) -> Cyclic {
    lhs += rhs;
    return lhs;
  }
  constexpr friend auto operator-(Cyclic lhs, const Cyclic &rhs) -> Cyclic {
    lhs -= rhs;
    return lhs;
  }

  // inline constexpr operator T() const { return this->m; }
  inline constexpr auto operator()() const -> T { return this->m; }
  constexpr auto operator<=>(const Cyclic &) const = default;

  constexpr friend auto operator<<(std::ostream &os,
                                            const Cyclic &obj) -> std::ostream & {
    os << std::to_string(obj.m);
    return os;
  }

  constexpr auto within(const Cyclic &a, double orb) -> bool {
    return ((a - *this) <= orb) || ((*this - a) <= orb);
  }

private:
  T m;
};

template <typename T, int MIN, int MAX>
constexpr auto
Cyclic<T, MIN, MAX>::operator+=(const Cyclic &rhs) -> Cyclic<T, MIN, MAX> & {
  this->m += rhs.m;
  while (this->m > MAX)
    this->m -= MAX;
  return *this;
}

template <typename T, int MIN, int MAX>
constexpr auto
Cyclic<T, MIN, MAX>::operator-=(const Cyclic &rhs) -> Cyclic<T, MIN, MAX> & {
  this->m -= rhs.m;
  while (this->m < MIN)
    this->m += MAX;
  return *this;
}

using Longitude = Cyclic<double, 0, 360>;

}; // namespace specni