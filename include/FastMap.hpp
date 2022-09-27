#pragma once

#include <algorithm>
#include <array>
#include <optional>
#include <ranges>

/*
 * Jason Turner's Constexpr Map
 */
template <typename Key, typename Value, std::size_t Size> struct FastMap {
  std::array<std::pair<Key, Value>, Size> data;

  [[nodiscard]] constexpr std::optional<Value> at(const Key &key) const {
    const auto iter = std::ranges::find_if(
        data, [&key](const auto &v) { return v.first == key; });

    if (iter != std::end(data)) {
      return iter->second;
    } else {
      return std::nullopt;
    }
  }
};
