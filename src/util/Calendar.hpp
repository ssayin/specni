#pragma once

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

namespace specni {
namespace util { // namespace specni

/*
  From chrono-Compatible Low-Level Date Algorithms
  Howard Hinnant
  2021-09-01
*/

// Returns year/month/day triple in civil calendar
// Preconditions:  z is number of days since 1970-01-01 and is in the range:
//                   [numeric_limits<Int>::min(),
//                   numeric_limits<Int>::max()-719468].
template <class Int>
constexpr std::tuple<Int, unsigned, unsigned> civil_from_days(Int z) noexcept {
  static_assert(
      std::numeric_limits<unsigned>::digits >= 18,
      "This algorithm has not been ported to a 16 bit unsigned integer");
  static_assert(
      std::numeric_limits<Int>::digits >= 20,
      "This algorithm has not been ported to a 16 bit signed integer");
  z += 719468;
  const Int era = (z >= 0 ? z : z - 146096) / 146097;
  const unsigned doe = static_cast<unsigned>(z - era * 146097); // [0, 146096]
  const unsigned yoe =
      (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
  const Int y = static_cast<Int>(yoe) + era * 400;
  const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
  const unsigned mp = (5 * doy + 2) / 153;                      // [0, 11]
  const unsigned d = doy - (153 * mp + 2) / 5 + 1;              // [1, 31]
  const unsigned m = mp < 10 ? mp + 3 : mp - 9;                 // [1, 12]
  return std::tuple<Int, unsigned, unsigned>(y + (m <= 2), m, d);
}

// Returns: true if y is a leap year in the civil calendar, else false
template <class Int> constexpr bool is_leap(Int y) noexcept {
  return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of common year
// The result is always in the range [28, 31].
constexpr unsigned last_day_of_month_common_year(unsigned m) noexcept {
  constexpr unsigned char a[] = {31, 28, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
  return a[m - 1];
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of leap year
// The result is always in the range [29, 31].
constexpr unsigned last_day_of_month_leap_year(unsigned m) noexcept {
  constexpr unsigned char a[] = {31, 29, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
  return a[m - 1];
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of year y
// The result is always in the range [28, 31].
template <class Int>
constexpr unsigned last_day_of_month(Int y, unsigned m) noexcept {
  return m != 2 || !is_leap(y) ? last_day_of_month_common_year(m) : 29u;
}

/*
  From chrono-Compatible Low-Level Date Algorithms
  Howard Hinnant
  2021-09-01
*/
std::tuple<unsigned, unsigned, unsigned, unsigned, unsigned, unsigned>
time_now();
} // namespace util
} // namespace specni