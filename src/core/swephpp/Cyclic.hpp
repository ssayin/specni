#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <ostream>

#include <cmath>
#include <string>

namespace specni {
template <std::floating_point T, int MIN, int MAX> class Cyclic {

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
  constexpr auto operator*=(const Cyclic &rhs) -> Cyclic &;
  constexpr auto operator-() -> Cyclic;

  template <std::floating_point U, int Mn, int Mx>
  constexpr friend auto operator+(Cyclic lhs, const Cyclic &rhs) -> Cyclic;

  template <std::floating_point U, int Mn, int Mx>
  constexpr friend auto operator-(Cyclic lhs, const Cyclic &rhs) -> Cyclic;

  template <std::floating_point U, int Mn, int Mx>
  constexpr friend auto operator*(Cyclic lhs, const Cyclic &rhs) -> Cyclic;

  constexpr auto operator<=>(const Cyclic &) const = default;

  template <std::floating_point U, int Mn, int Mx>
  constexpr friend std::tuple<Cyclic, Cyclic> ArcPair(Cyclic lhs, Cyclic rhs);

  template <std::floating_point U, int Mn, int Mx>
  constexpr friend auto operator<<(std::ostream &os,
                                   const Cyclic<U, Mn, Mx> &obj)
      -> std::ostream &;
};

template <std::floating_point T, int MIN, int MAX>
constexpr auto operator<<(std::ostream &os, const Cyclic<T, MIN, MAX> &obj)
    -> std::ostream & {
  os << std::to_string(obj.m);
  return os;
}

template <std::floating_point T, int MIN, int MAX>
constexpr void Cyclic<T, MIN, MAX>::ensure_within_range_max() {
  this->m = std::fmod(this->m, MAX);
}

template <std::floating_point T, int MIN, int MAX>
constexpr void Cyclic<T, MIN, MAX>::ensure_within_range_min() {
  this->m = std::fmod(std::fabs(MIN - this->m), MAX);
}

template <std::floating_point T, int MIN, int MAX>
constexpr auto Cyclic<T, MIN, MAX>::operator+=(const Cyclic &rhs)
    -> Cyclic<T, MIN, MAX> & {
  this->m += rhs.m;
  ensure_within_range_max();
  return *this;
}

template <std::floating_point T, int MIN, int MAX>
constexpr auto Cyclic<T, MIN, MAX>::operator-=(const Cyclic &rhs)
    -> Cyclic<T, MIN, MAX> & {
  this->m -= rhs.m;
  ensure_within_range_min();
  return *this;
}

template <std::floating_point T, int MIN, int MAX>
constexpr auto Cyclic<T, MIN, MAX>::operator*=(const Cyclic &rhs)
    -> Cyclic<T, MIN, MAX> & {
  this->m *= rhs.m;
  ensure_within_range_max();
  ensure_within_range_min();
  return *this;
}

template <std::floating_point T, int MIN, int MAX>
constexpr auto Cyclic<T, MIN, MAX>::operator-() -> Cyclic<T, MIN, MAX> {
  return Cyclic<T, MIN, MAX>(-(this->m)); // idk
}

template <std::floating_point T, int MIN, int MAX>
constexpr auto operator+(Cyclic<T, MIN, MAX> lhs,
                         const Cyclic<T, MIN, MAX> &rhs)
    -> Cyclic<T, MIN, MAX> {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T, int MIN, int MAX>
constexpr auto operator-(Cyclic<T, MIN, MAX> lhs,
                         const Cyclic<T, MIN, MAX> &rhs)
    -> Cyclic<T, MIN, MAX> {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T, int MIN, int MAX>
constexpr auto operator*(Cyclic<T, MIN, MAX> lhs,
                         const Cyclic<T, MIN, MAX> &rhs)
    -> Cyclic<T, MIN, MAX> {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T, int MIN, int MAX>
constexpr Cyclic<T, MIN, MAX>::Cyclic(const double o) {
  this->m = o;
  ensure_within_range_max();
  ensure_within_range_min();
}

template <std::floating_point T, int MIN, int MAX>
constexpr std::tuple<Cyclic<T, MIN, MAX>, Cyclic<T, MIN, MAX>>
ArcPair(Cyclic<T, MIN, MAX> lhs, Cyclic<T, MIN, MAX> rhs) {
  constexpr T epsilon = std::numeric_limits<T>::epsilon() * 1000;
  Cyclic<T, MIN, MAX> sub = lhs - rhs;
  return {sub, Cyclic<T, MIN, MAX>(MAX - static_cast<T>(sub))};
}

using EclipticLongitude = Cyclic<double, 0, 360>;
using EquatorialLatitude = Cyclic<double, -90, 90>;
}; // namespace specni
