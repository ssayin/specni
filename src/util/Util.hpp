#pragma once

#include <cmath>
#include <core/Aspects.hpp>
#include <string>
#include <utility>

#include <core/EssentialStates.hpp>

namespace specni {
namespace util {

inline constexpr float DegToRad(float deg) { return (M_PI * deg) / 180; }

const std::string get_sign_deg(/* longitude */ double lon);

inline const std::string AspectToString(Aspect asp) {
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

inline const std::string EssentialStateToString(EssentialState state) {
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
  }
}
} // namespace util
} // namespace specni
