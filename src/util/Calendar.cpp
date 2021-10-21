#include "Calendar.hpp"
std::tuple<unsigned, unsigned, unsigned, unsigned, unsigned, unsigned>
specni::util::time_now() {
  using namespace std;
  using namespace std::chrono;
  typedef duration<int, ratio_multiply<hours::period, ratio<24>>> days;
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
  auto us = duration_cast<microseconds>(now);
  return make_tuple(year, month, day, h.count(), m.count(), s.count());
}
