#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <type_traits>

namespace specni::core {

bool IsInBetween(auto a, auto b, auto val) {
  return (val++ - a) < ((b - a) + 1);
}

// initializer_list copies, don't feed large objs
template <class C> auto Min(C &&c) {
  return std::apply([](auto &&...args) { return std::min({args...}); },
                    std::forward<C>(c));
}

template <class Ch, class Tr, class... Args>
auto &operator<<(std::basic_ostream<Ch, Tr> &os, std::tuple<Args...> const &t) {
  std::apply([&os](auto &&...args) { ((os << args << " "), ...); }, t);
  return os;
}

} // namespace specni::core
