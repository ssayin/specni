#pragma once

#include <array>
#include <cstddef>

namespace specni {
enum class EssentialState {
  Domicile,
  MutualDomicile,
  InDetriment,
  Exalted,
  MutualExalted,
  Fallen,
  InOwnTriplicity,
  InOwnFace,
  InOwnTerm,
  Peregrine,
  Count
};

constexpr std::array<char, static_cast<size_t>(EssentialState::Count)>
    essentialScores{5, 5, -5, 4, 4, -4, 3, 1, 2, -5};

}; // namespace specni