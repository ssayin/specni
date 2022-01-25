#pragma once
#include "imgui.h"

#include <cmath>

namespace specni {
inline auto operator*(const ImVec2 &lhs, const float rhs) -> ImVec2 {
  return {lhs.x * rhs, lhs.y * rhs};
}
inline auto operator/(const ImVec2 &lhs, const float rhs) -> ImVec2 {
  return {lhs.x / rhs, lhs.y / rhs};
}
inline auto operator+(const ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}
inline auto operator-(const ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}
inline auto operator*(const ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 {
  return {lhs.x * rhs.x, lhs.y * rhs.y};
}
inline auto operator/(const ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 {
  return {lhs.x / rhs.x, lhs.y / rhs.y};
}
inline auto operator*=(ImVec2 &lhs, const float rhs) -> ImVec2 & {
  lhs.x *= rhs;
  lhs.y *= rhs;
  return lhs;
}
inline auto operator/=(ImVec2 &lhs, const float rhs) -> ImVec2 & {
  lhs.x /= rhs;
  lhs.y /= rhs;
  return lhs;
}
inline auto operator+=(ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 & {
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}
inline auto operator-=(ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 & {
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;
  return lhs;
}
inline auto operator*=(ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 & {
  lhs.x *= rhs.x;
  lhs.y *= rhs.y;
  return lhs;
}
inline auto operator/=(ImVec2 &lhs, const ImVec2 &rhs) -> ImVec2 & {
  lhs.x /= rhs.x;
  lhs.y /= rhs.y;
  return lhs;
}
inline auto operator+(const ImVec4 &lhs, const ImVec4 &rhs) -> ImVec4 {
  return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}
inline auto operator-(const ImVec4 &lhs, const ImVec4 &rhs) -> ImVec4 {
  return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}
inline auto operator*(const ImVec4 &lhs, const ImVec4 &rhs) -> ImVec4 {
  return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
}
inline auto ImRotate(const ImVec2 &v, float cos_a, float sin_a) -> ImVec2 {
  return {v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a};
}
}; // namespace specni
