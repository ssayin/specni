#include "util.hpp"

#include <include/cyclic.hpp>
#include <ostream>
#include <sstream>

const std::string specni::util::get_sign_deg(/* longitude */ double lon) {
  const static char *signs[] = {
      "Aries", "Taurus",  "Gemini",      "Cancer",    "Leo",      "Virgo",
      "Libra", "Scorpio", "Sagittarius", "Capricorn", "Aquarius", "Pisces",
  };

  int sign = lon / 30.0;
  double deg = std::fmod(lon, 30.0);

  std::stringstream ss;
  ss << signs[sign] << " " << longitude(deg);
  ss.flush();

  return ss.str();
}
