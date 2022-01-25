#include "Util.hpp"

namespace specni {
namespace util {
auto AspectToString(Aspect asp) -> const std::string {
  switch (asp) {
  case Aspect::Conjunction:
    return "Conjunction";
  case Aspect::Sextile:
    return "Sextile";
  case Aspect::Square:
    return "Square";
  case Aspect::Trine:
    return "Trine";
  case Aspect::Opposition:
    return "Opposition";
  default:
    return "";
  }
}

auto EssentialStateToString(EssentialState state) -> const std::string {
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
auto get_sign_deg(/* longitude */ double lon) -> const std::string {
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
