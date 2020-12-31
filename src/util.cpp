#include "util.h"

const std::pair<std::string, double>
swephpp::get_sign_deg(/* longitude */ double lon) {
  const static char *signs[] = {
      "Aries", "Taurus",  "Gemini",      "Cancer",    "Leo",      "Virgo",
      "Libra", "Scorpio", "Sagittarius", "Capricorn", "Aquarius", "Pisces",
  };

  int sign = lon / 30.0;
  double deg = std::fmod(lon, 30.0);

  return std::make_pair(std::string(signs[sign]), deg);
}
