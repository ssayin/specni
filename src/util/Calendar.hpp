#pragma once

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

namespace specni::util { // namespace specni

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
constexpr auto civil_from_days(Int z) noexcept
    -> std::tuple<Int, unsigned, unsigned> {
  static_assert(
      std::numeric_limits<unsigned>::digits >= 18,
      "This algorithm has not been ported to a 16 bit unsigned integer");
  static_assert(
      std::numeric_limits<Int>::digits >= 20,
      "This algorithm has not been ported to a 16 bit signed integer");
  z += 719468;
  const Int era = (z >= 0 ? z : z - 146096) / 146097;
  const auto doe = static_cast<unsigned>(z - era * 146097); // [0, 146096]
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
template <class Int> constexpr auto is_leap(Int y) noexcept -> bool {
  return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of common year
// The result is always in the range [28, 31].
constexpr auto last_day_of_month_common_year(unsigned m) noexcept -> unsigned {
  constexpr unsigned char a[] = {31, 28, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
  return a[m - 1];
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of leap year
// The result is always in the range [29, 31].
constexpr auto last_day_of_month_leap_year(unsigned m) noexcept -> unsigned {
  constexpr unsigned char a[] = {31, 29, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
  return a[m - 1];
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of year y
// The result is always in the range [28, 31].
template <class Int>
constexpr auto last_day_of_month(Int y, unsigned m) noexcept -> unsigned {
  return m != 2 || !is_leap(y) ? last_day_of_month_common_year(m) : 29u;
}

/*
  From chrono-Compatible Low-Level Date Algorithms
  Howard Hinnant
  2021-09-01
*/
static auto time_now()
    -> std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t> {
  using namespace std;
  using namespace std::chrono;
  using days = duration<int, ratio_multiply<hours::period, ratio<24>>>;
  // auto utc_offset = hours(0); // my current UTC offset
  // Get duration in local units
  auto now = system_clock::now().time_since_epoch(); //+ utc_offset;
  // Get duration in days
  auto today = duration_cast<days>(now);
  int year;
  unsigned month;
  unsigned day;
  // Convert days into year/month/day
  std::tie(year, month, day) = civil_from_days(today.count());
  // Subtract off days, leaving now containing time since local midnight
  now -= today;
  auto h = duration_cast<hours>(now);
  now -= h;
  auto m = duration_cast<minutes>(now);
  now -= m;
  auto s = duration_cast<seconds>(now);
  now -= s;
  // auto us = duration_cast<microseconds>(now);
  return make_tuple(year, month, day, h.count(), m.count(), s.count());
}

template <typename tuple_t>
constexpr auto get_array_from_tuple(tuple_t &&tuple) {
  constexpr auto get_array = [](auto &&...x) {
    return std::array{std::forward<decltype(x)>(x)...};
  };
  return std::apply(get_array, std::forward<tuple_t>(tuple));
}

} // namespace specni::util
