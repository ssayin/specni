#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <ostream>

#include <cmath>
#include <string>

namespace specni {
template <typename T, int MIN, int MAX> class Cyclic {
private:
  constexpr void ensure_within_range_max();
  constexpr void ensure_within_range_min();
  T m;

public:
  constexpr Cyclic() = default;
  constexpr virtual ~Cyclic() = default;
  constexpr Cyclic(const Cyclic &o) = default;

  constexpr Cyclic(const double o);
  explicit constexpr operator double() const { return this->m; }
  constexpr auto operator+=(const Cyclic &rhs) -> Cyclic &;
  constexpr auto operator-=(const Cyclic &rhs) -> Cyclic &;

  template <typename U, int Mn, int Mx>
  constexpr friend auto operator+(Cyclic lhs, const Cyclic &rhs) -> Cyclic;

  template <typename U, int Mn, int Mx>
  constexpr friend auto operator-(Cyclic lhs, const Cyclic &rhs) -> Cyclic;
  constexpr auto operator<=>(const Cyclic &) const = default;

  template <typename U, int Mn, int Mx>
  constexpr friend auto operator<<(std::ostream &os, const Cyclic &obj)
      -> std::ostream &;

  constexpr auto IsWithinOrbOf(const Cyclic &a, double orb) const -> bool;
};

template <typename T, int MIN, int MAX>
constexpr auto operator<<(std::ostream &os, const Cyclic<T, MIN, MAX> &obj)
    -> std::ostream & {
  os << std::to_string(obj.m);
  return os;
}

template <typename T, int MIN, int MAX>
constexpr auto Cyclic<T, MIN, MAX>::IsWithinOrbOf(const Cyclic &a,
                                                  double orb) const -> bool {
  return ((a - *this) <= orb) || ((*this - a) <= orb);
}

template <typename T, int MIN, int MAX>
constexpr void Cyclic<T, MIN, MAX>::ensure_within_range_max() {
  while (this->m > MAX)
    this->m -= MAX;
}

template <typename T, int MIN, int MAX>
constexpr void Cyclic<T, MIN, MAX>::ensure_within_range_min() {
  while (this->m < MIN)
    this->m += MAX;
}

template <typename T, int MIN, int MAX>
constexpr auto Cyclic<T, MIN, MAX>::operator+=(const Cyclic &rhs)
    -> Cyclic<T, MIN, MAX> & {
  this->m += rhs.m;
  ensure_within_range_max();
  return *this;
}

template <typename T, int MIN, int MAX>
constexpr auto Cyclic<T, MIN, MAX>::operator-=(const Cyclic &rhs)
    -> Cyclic<T, MIN, MAX> & {
  this->m -= rhs.m;
  ensure_within_range_min();
  return *this;
}

template <typename T, int MIN, int MAX>
constexpr auto operator+(Cyclic<T, MIN, MAX> lhs,
                         const Cyclic<T, MIN, MAX> &rhs)
    -> Cyclic<T, MIN, MAX> {
  lhs += rhs;
  return lhs;
}

template <typename T, int MIN, int MAX>
constexpr auto operator-(Cyclic<T, MIN, MAX> lhs,
                         const Cyclic<T, MIN, MAX> &rhs)
    -> Cyclic<T, MIN, MAX> {
  lhs -= rhs;
  return lhs;
}

template <typename T, int MIN, int MAX>
constexpr Cyclic<T, MIN, MAX>::Cyclic(const double o) {
  this->m = o;
  ensure_within_range_min();
  ensure_within_range_max();
}

using EclipticLongitude = Cyclic<double, 0, 360>;
using EquatorialLatitude = Cyclic<double, -90, 90>;
}; // namespace specni
