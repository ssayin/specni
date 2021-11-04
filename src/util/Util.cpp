#include "Util.hpp"

namespace specni {
namespace util {
const std::string AspectToString(Aspect asp) {
  switch (asp) {
  case Conjunction:
    return "Conjunction";
  case Sextile:
    return "Sextile";
  case Square:
    return "Square";
  case Trine:
    return "Trine";
  case Opposition:
    return "Opposition";
  default:
    return "";
  }
}

const std::string EssentialStateToString(EssentialState state) {
  switch (state) {
  case EssentialState::Domicile:
    return "Domicile";
  case EssentialState::MutualDomicile:
    return "Mutually Domicile";
  case EssentialState::InDetriment:
    return "In Detriment";
  case EssentialState::Exalted:
    return "Exalted";
  case EssentialState::MutualExalted:
    return "Mutually Exalted";
  case EssentialState::Fallen:
    return "Fallen";
  case EssentialState::InOwnTriplicity:
    return "In Own Triplicity";
  case EssentialState::InOwnFace:
    return "In Own Face";
  case EssentialState::InOwnTerm:
    return "In Own Term";
  case EssentialState::Peregrine:
    return "Peregrine";
  default:
    return "";
  }
}
const std::string get_sign_deg(/* longitude */ double lon) {
  std::stringstream ss;
  int deg;
  int min;
  int sec;
  std::tie(deg, min, sec) = degtodms(std::fmod(lon, 30.0));
  ss.fill('0');
  ss << deg << "° " << std::setw(2) << SignToString(static_cast<Sign>(lon / 30))
     << ' ' << std::setw(2) << min << '\'' << std::setw(2) << sec << "\"";

  return ss.str();
}
}; // namespace util
}; // namespace specni
