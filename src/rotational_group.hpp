#pragma once

#include <cmath>
#include <functional>
#include <iostream>

template <typename T, int MIN, int MAX> class rotational_group {
public:
  rotational_group() = default;
  virtual ~rotational_group() = default;
  rotational_group(const double &o) { this->m = o; }
  // rotational_group(double &o) { this->m = o; }

  rotational_group &operator=(const T &o) {
    this->m = o;
    return *this;
  }

  rotational_group &operator+=(const rotational_group &rhs);

  rotational_group &operator-=(const rotational_group &rhs);

  friend rotational_group operator+(rotational_group lhs,
                                    const rotational_group &rhs) {
    lhs += rhs;
    return lhs;
  }

  friend rotational_group operator-(rotational_group lhs,
                                    const rotational_group &rhs);

  friend inline bool operator<(const rotational_group &lhs,
                               const rotational_group &rhs) {
    return lhs.m < rhs.m;
  }
  friend inline bool operator>(const rotational_group &lhs,
                               const rotational_group &rhs) {
    return rhs < lhs;
  }
  friend inline bool operator<=(const rotational_group &lhs,
                                const rotational_group &rhs) {
    return !(lhs > rhs);
  }
  friend inline bool operator>=(const rotational_group &lhs,
                                const rotational_group &rhs) {
    return !(lhs < rhs);
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const rotational_group &obj) {
    os << obj.m;
    return os;
  }

  bool within(rotational_group &a, rotational_group &b, double orb) {
    return (a + orb) >= b;
  }

private:
  T m;
};

template <typename T, int MIN, int MAX>
rotational_group<T, MIN, MAX> &
rotational_group<T, MIN, MAX>::operator+=(const rotational_group &rhs) {
  this->m += rhs.m;
  while (this->m > MAX)
    this->m -= MAX;
  return *this;
}

template <typename T, int MIN, int MAX>
rotational_group<T, MIN, MAX> &
rotational_group<T, MIN, MAX>::operator-=(const rotational_group &rhs) {
  this->m -= rhs.m;
  this->m += MAX;
  while (this->m > MAX)
    this->m -= MAX;
  return *this;
}

rotational_group<double, -90, 90> latitude;
rotational_group<double, 0, 360> longitude;
