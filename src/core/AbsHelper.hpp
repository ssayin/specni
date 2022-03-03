#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <ranges>
#include <type_traits>

namespace specni {
namespace core {

template <typename T, typename U>
constexpr auto AddSub(T x, U y) -> std::tuple<T, T> {
  return {x + y, x - y};
}

template <typename T, typename U = T, typename V = T>
constexpr auto AddSub(T x, U y, V z) -> auto {
  return std::tuple_cat(AddSub(y - x, z), std::move(AddSub(x - y, z)));
}

template <typename T, typename U>
constexpr auto AddSubFabs(T x, U y) -> std::tuple<T, T> {
  return {std::fabs(x + y), std::fabs(x - y)};
}

// initializer_list copies, don't feed large objs
template <class C> auto Min(C &&c) {
  return std::apply([](auto &&...args) { return std::min({args...}); },
                    std::forward<C>(c));
}

}; // namespace core

}; // namespace specni
