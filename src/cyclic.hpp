#pragma once

#include <cmath>
#include <functional>
#include <iostream>

namespace specni {
template <typename T, int MIN, int MAX> class cyclic {
public:
  cyclic() = default;
  virtual ~cyclic() = default;
  cyclic(const double &o) { this->m = o; }
  cyclic(const cyclic &o) { *this = o; }

  cyclic &operator+=(const cyclic &rhs);

  cyclic &operator-=(const cyclic &rhs);

  friend cyclic operator+(cyclic lhs, const cyclic &rhs) {
    lhs += rhs;
    return lhs;
  }
  friend cyclic operator-(cyclic lhs, const cyclic &rhs) {
    lhs -= rhs;
    return lhs;
  }

  friend inline bool operator<(const cyclic &lhs, const cyclic &rhs) {
    return lhs.m < rhs.m;
  }
  friend inline bool operator>(const cyclic &lhs, const cyclic &rhs) {
    return rhs < lhs;
  }
  friend inline bool operator<=(const cyclic &lhs, const cyclic &rhs) {
    return !(lhs > rhs);
  }
  friend inline bool operator>=(const cyclic &lhs, const cyclic &rhs) {
    return !(lhs < rhs);
  }

  friend std::ostream &operator<<(std::ostream &os, const cyclic &obj) {
    os << obj.m;
    return os;
  }

  bool within(cyclic &a, int orb) {
    return ((a + *this) <= orb) || ((a - *this) <= orb) || ((*this - a) <= orb);
  }

private:
  T m;
};

template <typename T, int MIN, int MAX>
cyclic<T, MIN, MAX> &cyclic<T, MIN, MAX>::operator+=(const cyclic &rhs) {
  this->m += rhs.m;
  while (this->m > MAX)
    this->m -= MAX;
  return *this;
}

template <typename T, int MIN, int MAX>
cyclic<T, MIN, MAX> &cyclic<T, MIN, MAX>::operator-=(const cyclic &rhs) {
  this->m -= rhs.m;
  while (this->m < MIN)
    this->m += MAX;
  return *this;
}

typedef cyclic<double, -90, 90> latitude;
typedef cyclic<double, 0, 360> longitude;
}; // namespace specni