#ifndef SPECNI_IMGUIEXT_HPP
#define SPECNI_IMGUIEXT_HPP

#include "imgui.h"

namespace specni {

inline ImVec2 operator*(const ImVec2 &lhs, const float rhs) {
  return ImVec2(lhs.x * rhs, lhs.y * rhs);
}
inline ImVec2 operator/(const ImVec2 &lhs, const float rhs) {
  return ImVec2(lhs.x / rhs, lhs.y / rhs);
}
inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) {
  return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
inline ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs) {
  return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}
inline ImVec2 operator*(const ImVec2 &lhs, const ImVec2 &rhs) {
  return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}
inline ImVec2 operator/(const ImVec2 &lhs, const ImVec2 &rhs) {
  return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y);
}
inline ImVec2 &operator*=(ImVec2 &lhs, const float rhs) {
  lhs.x *= rhs;
  lhs.y *= rhs;
  return lhs;
}
inline ImVec2 &operator/=(ImVec2 &lhs, const float rhs) {
  lhs.x /= rhs;
  lhs.y /= rhs;
  return lhs;
}
inline ImVec2 &operator+=(ImVec2 &lhs, const ImVec2 &rhs) {
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}
inline ImVec2 &operator-=(ImVec2 &lhs, const ImVec2 &rhs) {
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;
  return lhs;
}
inline ImVec2 &operator*=(ImVec2 &lhs, const ImVec2 &rhs) {
  lhs.x *= rhs.x;
  lhs.y *= rhs.y;
  return lhs;
}
inline ImVec2 &operator/=(ImVec2 &lhs, const ImVec2 &rhs) {
  lhs.x /= rhs.x;
  lhs.y /= rhs.y;
  return lhs;
}
inline ImVec4 operator+(const ImVec4 &lhs, const ImVec4 &rhs) {
  return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
inline ImVec4 operator-(const ImVec4 &lhs, const ImVec4 &rhs) {
  return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
inline ImVec4 operator*(const ImVec4 &lhs, const ImVec4 &rhs) {
  return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
inline ImVec2 ImRotate(const ImVec2 &v, float cos_a, float sin_a) {
  return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
}
}; // namespace specni

#endif // SPECNI_IMGUIEXT_HPP
